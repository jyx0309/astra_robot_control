#!/usr/bin/env bash
set -eo pipefail
source /opt/ros/jazzy/setup.bash
ros2 service call /robot/stop std_srvs/srv/Trigger "{}"
