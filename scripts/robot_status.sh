#!/usr/bin/env bash
set -eo pipefail
source /opt/ros/jazzy/setup.bash
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
source "$PROJECT_ROOT/ros_ws/install/setup.bash"
printf '%s\n' '--- 节点 ---'
ros2 node list
printf '%s\n' '--- 机器人服务/动作 ---'
ros2 service list | grep -E '^/robot/' || true
ros2 action list | grep -E '^/robot/' || true
printf '%s\n' '--- 状态 ---'
ros2 topic echo /robot/state --once
