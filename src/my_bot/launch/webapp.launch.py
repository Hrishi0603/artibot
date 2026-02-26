import os
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():

    # 1. ROS Bridge Server
    rosbridge_node = Node(
        package='rosbridge_server',
        executable='rosbridge_websocket',
        name='rosbridge_websocket',
        output='screen',
        parameters=[{'use_sim_time': True}]
    )

    # 2. Web Video Server (for Camera Feed)
    video_server_node = Node(
        package='web_video_server',
        executable='web_video_server',
        name='web_video_server',
        output='screen',
        parameters=[{'port': 8080}]
    )

    return LaunchDescription([
        rosbridge_node,
        video_server_node
    ])