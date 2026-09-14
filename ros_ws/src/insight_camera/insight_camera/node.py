"""采集 RGB、左目和右目图像；不做深度几何或机器人运动决策。"""
from collections import deque
import json
from pathlib import Path
import shutil
import threading
import time
import uuid

import cv2
from cv_bridge import CvBridge
import numpy as np
import rclpy
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup, ReentrantCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node
from rclpy.qos import qos_profile_sensor_data
from sensor_msgs.msg import CompressedImage, Image
from std_srvs.srv import Trigger


def stamp_ns(msg):
    return msg.header.stamp.sec * 1_000_000_000 + msg.header.stamp.nanosec


def choose_views(frames, boundary, tolerance, names, arrival_times=None):
    """选择时间范围内的一组最新图像。"""
    def sync_stamp(name, index, message):
        if arrival_times is not None:
            return arrival_times[name][index]
        return stamp_ns(message)

    for rgb_index in range(len(frames['rgb']) - 1, -1, -1):
        rgb = frames['rgb'][rgb_index]
        if stamp_ns(rgb) <= boundary:
            continue
        selected = {'rgb': rgb}
        selected_indices = {'rgb': rgb_index}
        for name in names:
            if name == 'rgb':
                continue
            candidates = [(i, m) for i, m in enumerate(frames[name])
                          if stamp_ns(m) > boundary]
            if not candidates:
                break
            index, message = min(
                candidates,
                key=lambda item: abs(sync_stamp(name, item[0], item[1]) -
                                     sync_stamp('rgb', rgb_index, rgb)))
            selected[name] = message
            selected_indices[name] = index
        if len(selected) != len(names):
            continue
        stamps = [sync_stamp(name, selected_indices[name], selected[name])
                  for name in selected]
        if max(stamps) - min(stamps) <= tolerance:
            return selected
    return None


def rotate_image(image, degrees):
    if degrees == 90:
        return cv2.rotate(image, cv2.ROTATE_90_CLOCKWISE)
    if degrees == 180:
        return cv2.rotate(image, cv2.ROTATE_180)
    if degrees == 270:
        return cv2.rotate(image, cv2.ROTATE_90_COUNTERCLOCKWISE)
    return image


class ObservationNode(Node):
    def __init__(self, **kwargs):
        super().__init__('insight_observer', **kwargs)
        defaults = dict(
            rgb_topic='/camera/camera/color/image_rect_raw/compressed',
            rgb_transport='compressed',
            left_topic='/camera/camera/infra1/image_rect_raw',
            right_topic='/camera/camera/infra2/image_rect_raw',
            require_stereo=True,
            capture_timeout=5.0,
            sync_tolerance=0.08,
            image_rotation_deg=180,
            output_dir='~/insight_captures',
        )
        for name, value in defaults.items():
            self.declare_parameter(name, value)
        self.settings = {name: self.get_parameter(name).value for name in defaults}
        if self.settings['rgb_transport'] not in ('compressed', 'raw'):
            raise ValueError('rgb_transport 必须是 compressed 或 raw')
        if self.settings['capture_timeout'] <= 0 or self.settings['sync_tolerance'] < 0:
            raise ValueError('超时时间或同步容差无效')
        if self.settings['image_rotation_deg'] not in (0, 90, 180, 270):
            raise ValueError('image_rotation_deg 必须是 0、90、180 或 270')

        self.bridge = CvBridge()
        self.condition = threading.Condition()
        self.frames = {name: deque(maxlen=30) for name in ('left', 'right', 'rgb')}
        self.arrival_times = {name: deque(maxlen=30) for name in self.frames}
        self.sensor_group = ReentrantCallbackGroup()
        self.service_group = MutuallyExclusiveCallbackGroup()
        rgb_type = CompressedImage if self.settings['rgb_transport'] == 'compressed' else Image
        self.create_subscription(
            rgb_type, self.settings['rgb_topic'],
            lambda msg: self.receive('rgb', msg), qos_profile_sensor_data,
            callback_group=self.sensor_group)
        for name in ('left', 'right'):
            self.create_subscription(
                Image, self.settings[name + '_topic'],
                lambda msg, key=name: self.receive(key, msg),
                qos_profile_sensor_data, callback_group=self.sensor_group)
        self.create_service(Trigger, '~/capture', self.capture,
                           callback_group=self.service_group)
        self.get_logger().info('已就绪：~/capture 等待 RGB、左目和右目新图像。')

    def receive(self, name, msg):
        with self.condition:
            self.frames[name].append(msg)
            self.arrival_times[name].append(time.monotonic_ns())
            self.condition.notify_all()

    def capture(self, request, response):
        deadline = time.monotonic() + self.settings['capture_timeout']
        selected = None
        with self.condition:
            for frames in self.frames.values():
                frames.clear()
            for arrivals in self.arrival_times.values():
                arrivals.clear()
            while rclpy.ok(context=self.context):
                frames = {key: list(value) for key, value in self.frames.items()}
                arrival_times = {key: list(value)
                                 for key, value in self.arrival_times.items()}
                names = ['rgb']
                if self.settings['require_stereo']:
                    names += ['left', 'right']
                selected = choose_views(
                    frames, 0,
                    int(self.settings['sync_tolerance'] * 1e9),
                    names, arrival_times=arrival_times)
                if selected:
                    break
                remaining = deadline - time.monotonic()
                if remaining <= 0:
                    response.success = False
                    response.message = '没有新的观察结果：请检查 RGB、left/right 话题和时间同步。'
                    return response
                self.condition.wait(min(remaining, 0.1))
            else:
                response.success = False
                response.message = 'ROS 正在关闭'
                return response
        try:
            path = self.save(selected)
            response.success = True
            response.message = str(path)
        except Exception as exc:
            self.get_logger().error(f'采集失败：{exc}')
            response.success = False
            response.message = f'采集失败：{exc}'
        return response

    def save(self, selected):
        rgb = selected['rgb']
        if isinstance(rgb, CompressedImage):
            color = self.bridge.compressed_imgmsg_to_cv2(rgb, 'bgr8')
        elif rgb.encoding.lower() == 'nv12':
            if rgb.width % 2 or rgb.height % 2 or rgb.step < rgb.width:
                raise ValueError('NV12 尺寸/步长无效')
            raw = np.frombuffer(bytes(rgb.data), np.uint8).reshape(
                rgb.height * 3 // 2, rgb.step)
            color = cv2.cvtColor(np.ascontiguousarray(raw[:, :rgb.width]),
                                 cv2.COLOR_YUV2BGR_NV12)
        else:
            color = self.bridge.imgmsg_to_cv2(rgb, 'bgr8')
        if color is None or color.size == 0:
            raise ValueError('RGB 解码失败')
        color = rotate_image(color, self.settings['image_rotation_deg'])

        directory = (Path(self.settings['output_dir']).expanduser().resolve() /
                     uuid.uuid4().hex)
        directory.mkdir(parents=True, exist_ok=False)
        try:
            if not cv2.imwrite(str(directory / 'rgb.png'), color):
                raise OSError('无法保存 RGB')
            views = []
            for role in ('left', 'right', 'rgb'):
                if role != 'rgb':
                    msg = selected[role]
                    if msg.encoding not in ('mono8', '8UC1'):
                        raise ValueError(f'不支持的 {role} 编码：{msg.encoding}')
                    image = rotate_image(
                        self.bridge.imgmsg_to_cv2(msg, 'passthrough'),
                        self.settings['image_rotation_deg'])
                    if not cv2.imwrite(str(directory / (role + '.png')), image):
                        raise OSError(f'无法保存 {role}')
                msg = selected[role]
                views.append(dict(
                    role=role,
                    file=role + '.png',
                    stamp_ns=stamp_ns(msg),
                    frame_id=msg.header.frame_id,
                    topic=self.settings[role + '_topic']))
            metadata = dict(
                schema_version=3,
                views=views,
                camera_device='Insight9',
                image_rotation_deg=self.settings['image_rotation_deg'],
                view_span_ns=max(v['stamp_ns'] for v in views) -
                min(v['stamp_ns'] for v in views),
                robot_state=None,
                previous_action_result=None,
            )
            (directory / 'metadata.json').write_text(
                json.dumps(metadata, ensure_ascii=False, indent=2) + '\n')
        except Exception:
            shutil.rmtree(directory)
            raise
        return directory / 'metadata.json'


def main(args=None):
    rclpy.init(args=args)
    node = ObservationNode()
    executor = MultiThreadedExecutor(num_threads=3)
    executor.add_node(node)
    try:
        executor.spin()
    except KeyboardInterrupt:
        pass
    finally:
        executor.shutdown()
        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()
