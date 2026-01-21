import os

from ament_index_python.packages import get_package_share_directory


from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.actions import AppendEnvironmentVariable
from pathlib import Path
from launch.launch_description_sources import PythonLaunchDescriptionSource

from launch_ros.actions import Node

def generate_launch_description():
    package_name='my_bot' 

    world_path = os.path.join(
        get_package_share_directory(package_name),'worlds', 'empty.world'
    )

    set_gz_resource_path = AppendEnvironmentVariable(
        name='GZ_SIM_RESOURCE_PATH',
        value=os.path.join(Path.home(), '.gz', 'fuel', 'fuel.gazebosim.org', 'openrobotics', 'models')
    )

    # 1. Robot State Publisher
    rsp = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([os.path.join(
            get_package_share_directory(package_name),'launch','rsp.launch.py'
        )]), launch_arguments={'use_sim_time': 'true'}.items()
    )

    # 2. Gazebo Simulation
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([os.path.join(
            get_package_share_directory('ros_gz_sim'), 'launch', 'gz_sim.launch.py')]),
        launch_arguments={'gz_args': f'-r {world_path}'}.items()
    )

    # 3. Spawn the Robot
    spawn_entity = Node(package='ros_gz_sim', executable='create',
        arguments=['-topic', 'robot_description',
                   '-name', 'my_bot', 
                   '-allow_renaming', 'true'],
        output='screen')
    

    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        arguments=[
            '/clock@rosgraph_msgs/msg/Clock[gz.msgs.Clock',
            # '/cmd_vel@geometry_msgs/msg/Twist@gz.msgs.Twist',
            '/model/my_bot/scan@sensor_msgs/msg/LaserScan[gz.msgs.LaserScan',
            # '/model/my_bot/odometry@nav_msgs/msg/Odometry@gz.msgs.Odometry',
            # '/model/my_bot/tf@tf2_msgs/msg/TFMessage@gz.msgs.Pose_V',
            # '/model/my_bot/joint_states@sensor_msgs/msg/JointState@gz.msgs.Model', 
            # '/camera/image_raw@sensor_msgs/msg/Image@gz.msgs.Image',
            # '/camera/camera_info@sensor_msgs/msg/CameraInfo@gz.msgs.CameraInfo'
        ],

        remappings=[
            # ('/model/my_bot/odometry', '/odom'),
            # ('/model/my_bot/tf', '/tf'),
            # ('/model/my_bot/joint_states', '/joint_states'),
            ('/model/my_bot/scan', '/scan')
        ],
        output='screen'
    )

    return LaunchDescription([
        set_gz_resource_path,
        rsp,
        gazebo,
        spawn_entity,
        bridge, 
    ])