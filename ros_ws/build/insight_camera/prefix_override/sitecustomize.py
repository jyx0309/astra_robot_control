import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/yuxuan/astra_robot_control/skill/ros_ws/install/insight_camera'
