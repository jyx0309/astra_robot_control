#!/usr/bin/env python3
"""Archive observation/decision pairs without ROS dependencies or robot commands."""
import argparse
from contextlib import contextmanager
from datetime import datetime, timezone
import fcntl
import html
import json
import math
from pathlib import Path
import shutil
import tempfile
import uuid


def now():
    return datetime.now(timezone.utc).isoformat()


def atomic_write(path, text):
    with tempfile.NamedTemporaryFile(mode='w', encoding='utf-8',
                                     dir=path.parent, delete=False) as stream:
        temporary = Path(stream.name)
        stream.write(text)
    temporary.replace(path)


def write_json(path, value):
    atomic_write(path, json.dumps(value, ensure_ascii=False, indent=2) + '\n')


@contextmanager
def locked(session):
    if not (session / 'session.json').is_file():
        raise ValueError('请先用 init 创建记录目录')
    with (session / '.lock').open('a') as stream:
        fcntl.flock(stream, fcntl.LOCK_EX)
        yield


def read_optional(path):
    return Path(path).read_text(encoding='utf-8') if path else None


def render(session):
    info = json.loads((session / 'session.json').read_text(encoding='utf-8'))
    records = [json.loads(p.read_text(encoding='utf-8'))
               for p in sorted((session / 'steps').glob('*/record.json'))]
    esc = lambda value: html.escape(str(value))
    sections = []
    for record in records:
        step = record['step_id']
        images = ''.join(
            f'<figure><img loading="lazy" src="steps/{step}/{esc(v["file"])}">'
            f'<figcaption>{esc(v["role"])} · {esc(v.get("stamp_ns", ""))} ns'
            '</figcaption></figure>' for v in record['views'])
        details = ''.join(
            f'<details><summary>{label}</summary><pre>{esc(record.get(key) or "未记录")}</pre></details>'
            for key, label in [('state_text', '机器人状态'), ('action_text', '计划动作'),
                               ('result_text', '实际动作结果')])
        sections.append(
            f'<section><h2>{esc(step)}</h2><small>{esc(record["recorded_at"])}</small>'
            f'<div class="views">{images}</div><h3>决策摘要</h3>'
            f'<pre>{esc(record["summary"])}</pre>{details}</section>')
    page = ('<!doctype html><html lang="zh-CN"><meta charset="utf-8">'
            '<meta name="viewport" content="width=device-width,initial-scale=1">'
            '<title>机械臂推理记录</title><style>'
            'body{font:16px system-ui;background:#f3f5f7;margin:24px;color:#18212b}'
            'section{background:white;padding:20px;margin:20px 0;border-radius:12px}'
            '.views{display:flex;flex-wrap:wrap}figure{flex:1;min-width:220px;margin:8px}'
            'img{width:100%}pre{white-space:pre-wrap;overflow-wrap:anywhere}'
            'summary{cursor:pointer;padding:8px}small,figcaption{color:#536170}</style>'
            f'<h1>机械臂推理记录</h1><p>{esc(info["task"])}</p>'
            '<p>文字为显式记录的决策摘要。图像时间来自相机元数据；记录时间为 UTC。刷新查看新增步骤。</p>'
            + ''.join(sections) + '</html>')
    atomic_write(session / 'index.html', page)


def add(session, args):
    source = Path(args.observation).expanduser().resolve()
    metadata = json.loads(source.read_text(encoding='utf-8'))
    views = metadata.get('views', [])
    if not views:
        raise ValueError('观察元数据缺少 views')
    summary = args.summary if args.summary is not None else read_optional(args.summary_file)
    if not summary or not summary.strip():
        raise ValueError('决策摘要不能为空')
    step = datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ') + '-' + uuid.uuid4().hex[:8]
    stage = Path(tempfile.mkdtemp(prefix='.pending-', dir=session))
    try:
        saved_views = []
        for i, view in enumerate(views):
            image = (source.parent / view['file']).resolve()
            if not image.is_relative_to(source.parent):
                raise ValueError('图像必须位于观察目录内')
            if image.suffix.lower() not in ('.png', '.jpg', '.jpeg'):
                raise ValueError('仅支持 PNG/JPEG 图像')
            name = f'view_{i}{image.suffix.lower()}'
            shutil.copyfile(image, stage / name)
            saved_views.append({**view, 'file': name})
        shutil.copyfile(source, stage / 'source_metadata.json')
        record = dict(schema_version=1, step_id=step, recorded_at=now(),
                      source_observation=str(source), views=saved_views, summary=summary,
                      state_text=read_optional(args.state_file),
                      action_text=read_optional(args.action_file),
                      result_text=read_optional(args.result_file))
        write_json(stage / 'record.json', record)
        stage.rename(session / 'steps' / step)
    finally:
        if stage.exists():
            shutil.rmtree(stage)
    return step


def video(session, seconds=3.0, font_path=None):
    """Render recorded observations with burned-in, paginated Chinese captions."""
    if not math.isfinite(seconds) or seconds <= 0:
        raise ValueError('每页播放秒数必须为有限正数')
    try:
        import cv2
        import numpy as np
        from PIL import Image, ImageDraw, ImageFont, ImageOps
    except ImportError as exc:
        raise ValueError('视频导出需要 Pillow、numpy 和 opencv-python') from exc
    font_path = font_path or '/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc'
    font = ImageFont.truetype(str(font_path), 26)
    small = ImageFont.truetype(str(font_path), 20)
    records = sorted((session / 'steps').glob('*/record.json'))
    if not records:
        raise ValueError('会话还没有观察帧，无法导出视频')
    with tempfile.TemporaryDirectory(prefix='.video-', dir=session) as tmp:
        target = Path(tmp) / 'replay.mp4'
        writer = cv2.VideoWriter(str(target), cv2.VideoWriter_fourcc(*'mp4v'), 10, (1280, 720))
        if not writer.isOpened():
            raise ValueError('OpenCV 无法打开 MP4 编码器')
        try:
            for index, path in enumerate(records, 1):
                record = json.loads(path.read_text(encoding='utf-8'))
                canvas = Image.new('RGB', (1280, 720), '#18212b')
                draw = ImageDraw.Draw(canvas)
                draw.text((24, 15), f'观察 {index}/{len(records)}  ·  {record["recorded_at"]}', font=small, fill='white')
                views = record['views']
                if not 1 <= len(views) <= 3:
                    raise ValueError('视频支持每次观察 1–3 路图像')
                width = 1232 // len(views)
                for i, view in enumerate(views):
                    with Image.open(path.parent / view['file']) as source:
                        picture = ImageOps.contain(source.convert('RGB'), (width - 12, 360))
                    x = 24 + i * width
                    canvas.paste(picture, (x + (width - picture.width) // 2, 68 + (360 - picture.height) // 2))
                    draw.text((x, 435), str(view['role']), font=small, fill='#a5cfff')
                lines = []
                for paragraph in ('决策摘要：' + record['summary']).splitlines():
                    line = ''
                    for character in paragraph:
                        if line and draw.textlength(line + character, font=font) > 1220:
                            lines.append(line)
                            line = ''
                        line += character
                    lines.append(line)
                pages = [lines[i:i + 5] for i in range(0, len(lines), 5)]
                for page_number, page in enumerate(pages, 1):
                    frame = canvas.copy()
                    painter = ImageDraw.Draw(frame)
                    for line_number, line in enumerate(page):
                        painter.text((24, 482 + line_number * 35), line, font=font, fill='white')
                    painter.text((24, 685), f'摘要 {page_number}/{len(pages)} · 每页 {seconds:g} 秒 · 观察帧回放', font=small, fill='#a5cfff')
                    pixels = cv2.cvtColor(np.asarray(frame), cv2.COLOR_RGB2BGR)
                    for _ in range(max(1, round(seconds * 10))):
                        writer.write(pixels)
        finally:
            writer.release()
        check = cv2.VideoCapture(str(target))
        try:
            readable, _ = check.read()
            if not readable:
                raise ValueError('生成的视频无法读取')
        finally:
            check.release()
        target.replace(session / 'replay.mp4')
    return session / 'replay.mp4'


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest='command', required=True)
    init = commands.add_parser('init', help='创建独立记录目录')
    init.add_argument('session', type=Path)
    init.add_argument('--task', default='')
    append = commands.add_parser('add', help='归档一次模型观察和决策摘要')
    append.add_argument('session', type=Path)
    append.add_argument('--defer-render', action='store_true', help='仅归档；结束时 export/video 统一渲染')
    append.add_argument('--observation', required=True, help='相机返回的 metadata.json')
    summary = append.add_mutually_exclusive_group(required=True)
    summary.add_argument('--summary')
    summary.add_argument('--summary-file')
    for name in ('state', 'action', 'result'):
        append.add_argument(f'--{name}-file')
    result = commands.add_parser('result', help='按步骤编号补充实际动作结果')
    result.add_argument('session', type=Path)
    result.add_argument('--defer-render', action='store_true')
    result.add_argument('step_id')
    result.add_argument('--result-file', required=True)
    export = commands.add_parser('export', help='重建回放页面')
    export.add_argument('session', type=Path)
    movie = commands.add_parser('video', help='直接导出带中文摘要的 replay.mp4')
    movie.add_argument('session', type=Path)
    movie.add_argument('--seconds', type=float, default=3.0, help='每页秒数，默认 3')
    movie.add_argument('--font', help='支持中文的字体路径')
    args = parser.parse_args()
    session = args.session.expanduser().resolve()
    try:
        if args.command == 'init':
            session.mkdir(parents=True, exist_ok=False)
            (session / 'steps').mkdir()
            write_json(session / 'session.json', dict(schema_version=1, task=args.task, created_at=now()))
            render(session)
            print(session / 'index.html')
            return
        with locked(session):
            if args.command == 'video':
                render(session)
                print(video(session, args.seconds, args.font))
                return
            if args.command == 'add':
                step = add(session, args)
                print(step, flush=True)
            elif args.command == 'result':
                if Path(args.step_id).name != args.step_id or args.step_id in ('.', '..'):
                    raise ValueError('无效的步骤编号')
                path = session / 'steps' / args.step_id / 'record.json'
                record = json.loads(path.read_text(encoding='utf-8'))
                if record.get('result_text') is not None:
                    raise ValueError('该步骤已记录结果，不覆盖已有记录')
                record.update(result_text=read_optional(args.result_file), result_recorded_at=now())
                write_json(path, record)
            if not getattr(args, 'defer_render', False):
                render(session)
    except (OSError, ValueError, KeyError, TypeError) as exc:
        parser.exit(1, f'记录失败：{exc}\n')


if __name__ == '__main__':
    main()
