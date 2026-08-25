cmake_minimum_required(VERSION 3.8)
project(coverage_commander)

# Default to C++17 (or whatever your other packages use — check straightline_planner's CMakeLists for consistency)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(nav2_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp_action REQUIRED)

include_directories(
  include
)

add_executable(coverage_node
  src/main.cpp
  src/coverage_node.cpp
  src/row_scanner.cpp
)

target_link_libraries(coverage_node PUBLIC
  ${geometry_msgs_TARGETS}
  ${nav_msgs_TARGETS}
  ${nav2_msgs_TARGETS}
  ${rclcpp_action_TARGETS}
  rclcpp::rclcpp
)

install(TARGETS
  coverage_node
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()