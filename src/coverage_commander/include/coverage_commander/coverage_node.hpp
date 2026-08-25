#include <chrono>
#include <memory>
#include <string>

#ifndef COVERAGE_COMMANDER__COVERAGE_NODE_HPP_
#define COVERAGE_COMMANDER__COVERAGE_NODE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/occupancy_grid.hpp"
#include <geometry_msgs/msg/pose_stamped.hpp>
#include "rclcpp_action/rclcpp_action.hpp"
#include "nav2_msgs/action/follow_waypoints.hpp"

class CoverageNode : public rclcpp::Node{
public:
    CoverageNode();


private:
    
    void map_callback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg);
    rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr subscription_;
    bool coverage_started_{false};
    rclcpp_action::Client<nav2_msgs::action::FollowWaypoints>::SharedPtr action_client_;
    
};

#endif

