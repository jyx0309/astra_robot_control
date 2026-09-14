from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory
from pathlib import Path

def generate_launch_description():
 robot_config = str(
     Path(get_package_share_directory('robot_bringup')) / 'config/system.yaml'
 )
 camera_config = str(
     Path(get_package_share_directory('insight_camera')) / 'config/camera.yaml'
 )

 return LaunchDescription([
     DeclareLaunchArgument(
         'backend',
         default_value='mock',
         description='Execution backend: mock or carm.',
     ),
     DeclareLaunchArgument(
         'robot_ip',
         default_value='10.42.0.101',
         description='CARM controller IP address.',
     ),
     DeclareLaunchArgument(
         'hardware_config_verified',
         default_value='false',
         description='Explicit operator confirmation for real hardware config.',
     ),
     DeclareLaunchArgument(
         'speed_level',
         default_value='2.0',
         description='CARM speed level (0-2.5 in this project).',
     ),
     DeclareLaunchArgument(
         'camera',
         default_value='true',
         description='Start the Insight9 observation node.',
     ),
     Node(
         package='robot_execution',
         executable='execution_node',
         name='robot_execution',
         output='screen',
         parameters=[
             robot_config,
             {
                 'backend': LaunchConfiguration('backend'),
                 'robot_ip': LaunchConfiguration('robot_ip'),
                 'hardware_config_verified': LaunchConfiguration(
                     'hardware_config_verified'
                 ),
                 'speed_level': LaunchConfiguration('speed_level'),
             },
         ],
     ),
     Node(
         package='insight_camera',
         executable='observation_node',
         name='insight_observer',
         output='screen',
         condition=IfCondition(LaunchConfiguration('camera')),
         parameters=[camera_config],
     ),
 ])
