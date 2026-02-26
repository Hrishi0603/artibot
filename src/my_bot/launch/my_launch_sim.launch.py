import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, AppendEnvironmentVariable
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration

from launch_ros.actions import Node

def generate_launch_description():
    # 1. Setup paths and variables
    package_name = 'my_bot'
    pkg_project_bot = get_package_share_directory(package_name)
    
    # Use a launch configuration for sim time so all nodes stay in sync
    use_sim_time = LaunchConfiguration('use_sim_time')
    use_ros2_control = LaunchConfiguration('use_ros2_control')

    # 2. Robot State Publisher (RSP)    
    # This included file handles your camera.xacro and URDF processing
    rsp = IncludeLaunchDescription(
                PythonLaunchDescriptionSource([os.path.join(
                    get_package_share_directory(package_name),'launch','rsp.launch.py'
                )]), launch_arguments={'use_sim_time': 'true', 'use_ros2_control': 'true'}.items()
    )

    # 3. Gazebo Simulation Setup
    # Path to your NEW maze world
    default_world = os.path.join(pkg_project_bot, 'worlds', 'maze.world')    
    
    world = LaunchConfiguration('world')

    world_arg = DeclareLaunchArgument(
        'world',
        default_value=default_world,
        description='World to load'
    )
    
    # AUTOMATIC PATH SETTING: This tells Gazebo where to find your 'Maze_ql_1' folder
    # This replaces the manual 'export' command in the terminal
    set_gz_resource_path = AppendEnvironmentVariable(
        'GZ_SIM_RESOURCE_PATH',
        os.path.join(pkg_project_bot, 'models')
    )

    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([os.path.join(
            get_package_share_directory('ros_gz_sim'), 'launch', 'gz_sim.launch.py')]),
            launch_arguments={'gz_args': ['-r -v4 ', world], 'on_exit_shutdown': 'true'}.items()
    )

    # 4. Spawn Entity
    # Places the robot in the maze. You might need to adjust -x or -y if it spawns in a wall
    spawn_entity = Node(
        package='ros_gz_sim', 
        executable='create',
        arguments=['-topic', 'robot_description',
                   '-name', 'my_bot',
                   '-z', '0.1'],
        output='screen'
    )

    # 5. Bridges
    bridge_params = os.path.join(pkg_project_bot, 'config', 'gz_bridge.yaml')
    ros_gz_bridge = Node(
        package="ros_gz_bridge",
        executable="parameter_bridge",
        arguments=[
            '--ros-args',
            '-p',
            f'config_file:={bridge_params}',
        ]
    )

    ros_gz_image_bridge = Node(
        package="ros_gz_image",
        executable="image_bridge",
        arguments=["/camera/image_raw"]
    )

    diff_drive_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=[
            "diff_cont",
            '--controller-ros-args',
            '-r /diff_cont/cmd_vel:=/cmd_vel'
        ],
    )

    joint_broad_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_broad"],
    )
    
    twist_mux_config = os.path.join(get_package_share_directory(package_name),
                                         'config', 'twist_mux.yaml')
    twist_mux = Node(
        package='twist_mux',
        executable='twist_mux',
        output='screen',
        remappings={('/cmd_vel_out', '/cmd_vel')},
        parameters=[
            {'use_sim_time': True},
            twist_mux_config])

    return LaunchDescription([
        # Set Environment Variable first so Gazebo finds the maze immediately
        set_gz_resource_path,
        
        DeclareLaunchArgument(
            'use_sim_time',
            default_value='true',
            description='Use simulation (Gazebo) clock if true'),
        DeclareLaunchArgument(
            'use_ros2_control',
            default_value='true',
            description='Use ros2_control if true'),    
        world_arg,
        rsp,
        gazebo,
        spawn_entity,
        ros_gz_bridge,
        ros_gz_image_bridge,
        diff_drive_spawner,
        joint_broad_spawner,
        twist_mux,
    ])