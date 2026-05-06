# Autonomous Differential Drive Robot Simulation

![ROS 2](https://img.shields.io/badge/ROS_2-Kilted-blue.svg)
![Gazebo](https://img.shields.io/badge/Gazebo-Ignition_Sim-orange.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

A complete ROS 2 simulation of a differential drive robot navigating complex maze environments. This project integrates a custom URDF with `ros2_control` for precise kinematics in Gazebo Ignition, utilizes the SLAM Toolbox for real-time 2D mapping, and leverages the Nav2 stack for autonomous path planning and obstacle avoidance.

<!-- ## 🎥 Demonstration

> **[Insert GIF/Video of robot navigating the maze in Gazebo here]**

> **[Insert RViz2 Screenshot showing the 2D occupancy grid and costmaps here]** -->

## 🏗️ System Architecture & Features

* **Custom Kinematics:** Developed a full robot description (URDF) integrated with `ros2_control` hardware interfaces to ensure accurate velocity simulation in Gazebo Ignition.
* **Autonomous Navigation:** Configured the **Nav2** stack to execute collision-free dynamic pathing through simulated constraints. Custom tuning applied to the controller frequency and costmap inflation radii (restricted global, relaxed local) for optimal tight-space navigation.
* **Dynamic Mapping:** Implemented **SLAM Toolbox** (Online Async) to generate high-resolution 2D occupancy grids using simulated sensor data.
* **Control Multiplexing:** Utilized `twist_mux` to manage concurrent velocity commands. The priority hierarchy ensures safe operational hand-offs:
  1. Web Application Control (Highest Priority)
  2. Keyboard Teleoperation (`teleop_twist_keyboard`)
  3. Nav2 Autonomous Goals (Lowest Priority)

## 💻 Tech Stack

* **Core:** ROS 2 Kilted, C++, Python
* **Simulation:** Gazebo Ignition / gz sim, RViz2
* **Navigation & Control:** Nav2, SLAM Toolbox, `ros2_control`, `twist_mux`

## ⚙️ Prerequisites

To build and run this simulation, you will need the following installed:

* ROS 2 Kilted
* Gazebo Ignition / gz sim
* Navigation2 and SLAM Toolbox packages
* `ros2_control` and `teleop_twist_keyboard`

## 🚀 Installation & Build

Clone the repository into your ROS 2 workspace and build the packages:

```bash
# Clone the repository into your workspace's src directory
git clone https://github.com/Hrishi0603/artibot.git

# Navigate to your workspace root and build
colcon build --symlink-install
```

## 🎮 Usage & Runbook
Open separate terminals for each of the following steps. You must source the ROS 2 environment and your local workspace in every new terminal before running these commands:

```Bash
source /opt/ros/kilted/setup.bash
source install/setup.bash
```

#### 1. Launch the Gazebo Simulation:
This command starts the Gazebo Ignition environment, loads the custom robot URDF, and initializes the ros2_control hardware interfaces.
```Bash
ros2 launch my_bot launch_sim.launch.py
```
#### 2. Start Manual Control (Keyboard Teleop)
This launches manual control via twist_mux, ensuring proper frame mapping (base_link) and synchronizing with the simulation time.
```Bash
ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args -p stamped:=true -r /cmd_vel:=/cmd_vel_keyboard -p frame_id:=base_link -p use_sim_time:=true
```
#### 3. Launch SLAM Toolbox
This starts asynchronous online mapping to generate the 2D occupancy grid dynamically from simulated LiDAR/sensor data.
```Bash
ros2 launch slam_toolbox online_async_launch.py params_file:=src/my_bot/config/mapper_params_online_async.yaml use_sim_time:=true
```
#### 4. Start Visualization
This opens RViz2 so you can visualize the TF tree, laser scans, map generation, and Nav2 costmaps.
```Bash
rviz2
```
#### 5. Start Nav2 Stack
This launches the Navigation2 stack for autonomous path planning, applying custom tuned parameters for the global/local costmap inflation radii.
```Bash
ros2 launch nav2_bringup navigation_launch.py params_file:=config/nav2_params.yaml use_sim_time:=true
```

## 🗺️ Roadmap / Future Work
Dockerization: Containerize the entire ROS 2 and Gazebo Ignition environment using Docker to ensure seamless cross-platform reproducibility and dependency management.

Ackermann Kinematics Scaling: Transition the core navigation and control concepts from this differential drive base to a full-scale simulated autonomous Golf Cart.

Urban Environment Simulation: Deploy the scaled Golf Cart model into a comprehensive "SimCity" style urban Gazebo environment to test advanced lane-following, dynamic obstacle avoidance, and intersection navigation.

## 📬 Contact

**Hrishikesh Birje** 
* **LinkedIn:** [hrishikesh-birje](https://linkedin.com/in/hrishikesh-birje-641b69228)
* **GitHub:** [Hrishi0603](https://github.com/Hrishi0603)
