// coverage_node.cpp
#include "coverage_commander/coverage_node.hpp"
#include "coverage_commander/row_scanner.hpp"


CoverageNode::CoverageNode() : Node("coverage_node") {

  RCLCPP_INFO(this->get_logger(), "coverage node started");
  subscription_ = create_subscription<nav_msgs::msg::OccupancyGrid>("/map", 10, std::bind(&CoverageNode::map_callback, this,  std::placeholders::_1));
  action_client_ = rclcpp_action::create_client<nav2_msgs::action::FollowWaypoints>(this, "follow_waypoints");
  if (!action_client_->wait_for_action_server(std::chrono::seconds(5))) {
    RCLCPP_ERROR(this->get_logger(), "Action server not available");
    return; //explain
  }

}


void CoverageNode::map_callback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg){
  
  RCLCPP_INFO(this->get_logger(), "Coverage On");

  if(coverage_started_) return;  //set a timer and callback to check latest/better map

  RCLCPP_INFO(this->get_logger(), "Map received");

  this->declare_parameter("botsize", 6);

  const nav_msgs::msg::OccupancyGrid & grid = *msg;
  std::vector<std::pair<int, int>> start_p;
  std::vector<std::pair<int, int>> goal_p;
  int footprint = this->get_parameter("botsize");
  RCLCPP_INFO(this->get_logger(), "Botsize =", footprint);

  coverage_started_ = true;
  generategrid(grid, start_p, goal_p, footprint); //make this async and multithreaded
  RCLCPP_INFO(this->get_logger(), "generated %zu segments", start_p.size());
  
  std::vector<geometry_msgs::msg::PoseStamped> waypoints;
  geometry_msgs::msg::PoseStamped start_pose;
  geometry_msgs::msg::PoseStamped goal_pose;

  for(size_t i = 0; i<start_p.size(); i++){
    
    double world_x = grid.info.origin.position.x + (start_p[i].first + 0.5) * grid.info.resolution;
    double world_y = grid.info.origin.position.y + (start_p[i].second + 0.5) * grid.info.resolution;
    
    start_pose.header.frame_id = "map";
    start_pose.header.stamp = this->now();
    start_pose.pose.position.x = world_x;
    start_pose.pose.position.y = world_y;
    start_pose.pose.position.z = 0.0;
    start_pose.pose.orientation.w = 1.0; //compare world_x of start_pose and goal_pose to determine the orientation(w). what if we traverse columns instead of rows?
    waypoints.push_back(start_pose);

    world_x = grid.info.origin.position.x + (goal_p[i].first + 0.5) * grid.info.resolution;
    world_y = grid.info.origin.position.y + (goal_p[i].second + 0.5) * grid.info.resolution;
    
    goal_pose.header.frame_id = "map";
    goal_pose.header.stamp = this->now();
    goal_pose.pose.position.x = world_x;
    goal_pose.pose.position.y = world_y;
    goal_pose.pose.position.z = 0.0;
    goal_pose.pose.orientation.w = 1.0;
    waypoints.push_back(goal_pose);

  }

  auto goal_msg = nav2_msgs::action::FollowWaypoints::Goal();
  goal_msg.poses = waypoints;



  auto send_goal_options = rclcpp_action::Client<nav2_msgs::action::FollowWaypoints>::SendGoalOptions();

  send_goal_options.feedback_callback = 
    [this](auto, const auto feedback) {
      RCLCPP_INFO(this->get_logger(), "Currently navigating to waypoint: %d", 
        feedback->current_waypoint);
    };

  send_goal_options.result_callback = 
    [this](const auto & result) {
      RCLCPP_INFO(this->get_logger(), "Coverage complete! Missed waypoints: %zu", 
        result.result->missed_waypoints.size());
    };

  action_client_->async_send_goal(goal_msg, send_goal_options); //explain the problem

}


 