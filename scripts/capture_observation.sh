#!/usr/bin/env bash
set -eo pipefail
source /opt/ros/jazzy/setup.bash
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
source "$PROJECT_ROOT/ros_ws/install/setup.bash"
timeout 15 ros2 service call /insight_observer/capture std_srvs/srv/Trigger "{}"
