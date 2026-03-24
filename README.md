Autonomous Differential Drive Robot (ROS 2 & Gazebo)
This repository contains the simulation of an autonomous differential drive robot built using ROS 2 and Gazebo. The project integrates sensor fusion (LiDAR and Camera) to perform SLAM (Simultaneous Localization and Mapping) and autonomous navigation in a simulated environment.

Overview
The goal of this project was to develop a robust simulation environment for a two-wheeled differential drive robot. By leveraging the ROS 2 framework, the robot is capable of mapping unknown environments and navigating to goal poses while avoiding obstacles in real-time.

Key Features
Differential Drive Kinematics: Custom URDF model with diff_drive_controller.

Sensory Integration: 360° 2D LiDAR for depth sensing and an RGB camera for visual feedback.

SLAM: Integrated slam_toolbox for high-quality map generation.

Navigation: Full implementation of the Nav2 stack for path planning and obstacle avoidance.

Gazebo Simulation: A custom environment designed to test sensor reliability and navigation accuracy.

Tech Stack
Middleware: ROS 2 (Kilted)

Simulator: Gazebo

Visualization: RViz2

Languages: Python, C++, XML (URDF/Xacro)

Algorithms: SLAM Toolbox, Nav2 (Smac/NavFn Planners)

Project Structure
Plaintext
├── description/            # URDF and Xacro files for robot model
├── launch/                 # ROS 2 launch files for simulation and sensors
├── config/                 # Configuration files (Nav2, Slam, Controller)
├── worlds/                 # Gazebo world files
├── maps/                   # Saved maps generated via SLAM
└── CMakeLists.txt/Package.xml
Installation & Setup
Clone the Repository:

Bash
mkdir -p ~/dev_ws/src
cd ~/dev_ws/src
git clone https://github.com/yourusername/diff_drive_bot.git
Install Dependencies:
Ensure you have ROS 2 and the necessary navigation packages installed:

Bash
sudo apt update
sudo apt install ros-$ROS_DISTRO-navigation2 ros-$ROS_DISTRO-nav2-bringup ros-$ROS_DISTRO-slam-toolbox
Build the Workspace:

Bash
cd ~/dev_ws
rosdep install --from-paths src --ignore-src -r -y
colcon build --symlink-install
source install/setup.bash
How to Run
1. Launch the Simulation
To spawn the robot in the Gazebo world and start the robot state publisher:

Bash
ros2 launch my_bot launch_sim.launch.py
2. Start Mapping (SLAM)
In a new terminal, run the SLAM node:

Bash
ros2 launch diff_drive_bot online_async_launch.py
3. Autonomous Navigation
Once a map is generated or loaded, launch the navigation stack:

Bash
ros2 launch diff_drive_bot navigation_launch.py
📈 Future Improvements
Implement EKF (Extended Kalman Filter) for better sensor fusion between IMU and Odometry.

Integrate Computer Vision (YOLO) via the camera sensor for object detection.

Transition from simulation to hardware deployment using a Raspberry Pi.
