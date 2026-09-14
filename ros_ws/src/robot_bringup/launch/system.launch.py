from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
from pathlib import Path

def generate_launch_description():
 config=str(Path(get_package_share_directory('robot_bringup'))/'config/system.yaml')
 return LaunchDescription([
  DeclareLaunchArgument('camera', default_value='true'),
  Node(package='robot_execution',executable='execution_node',name='robot_execution',output='screen',parameters=[config]),
  Node(package='insight_camera',executable='observation_node',name='insight_observer',output='screen',parameters=[config]),
 ])
