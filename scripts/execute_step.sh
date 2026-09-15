#!/usr/bin/env bash
set -eo pipefail
if [[ $# -lt 2 ]]; then
  echo "用法：execute_step.sh set_gripper OPENING_M [FORCE_N] [COMMAND_ID]" >&2
  echo "      execute_step.sh move_to X Y Z QX QY QZ QW [COMMAND_ID]" >&2
  exit 2
fi
source /opt/ros/jazzy/setup.bash
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
source "$PROJECT_ROOT/ros_ws/install/setup.bash"
kind="$1"; shift
command_id="skill-$(cat /proc/sys/kernel/random/uuid)"
case "$kind" in
  set_gripper)
    opening="$1"; force="${2:-2.0}"; command_id="${3:-$command_id}"
    ros2 action send_goal /robot/execute_step robot_interfaces/action/ExecuteStep \
      "{command_id: $command_id, kind: set_gripper, opening_m: $opening, force_n: $force, timeout_sec: 50.0}"
    ;;
  move_to|move_linear)
    [[ $# -ge 7 ]] || { echo "move 需要 X Y Z QX QY QZ QW" >&2; exit 2; }
    x="$1"; y="$2"; z="$3"; qx="$4"; qy="$5"; qz="$6"; qw="$7"; command_id="${8:-$command_id}"
    ros2 action send_goal /robot/execute_step robot_interfaces/action/ExecuteStep \
      "{command_id: $command_id, kind: $kind, timeout_sec: 50.0, target: {header: {frame_id: base_link}, pose: {position: {x: $x, y: $y, z: $z}, orientation: {x: $qx, y: $qy, z: $qz, w: $qw}}}}"
    ;;
  *) echo "不支持的动作：$kind" >&2; exit 2;;
esac
