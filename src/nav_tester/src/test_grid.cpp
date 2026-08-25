#include <rclcpp/rclcpp.hpp>
#include <vector>
#include <utility>
#include <iostream>
#include <cmath>
#include "nav2_costmap_2d/cost_values.hpp"
#include "nav2_costmap_2d/costmap_2d.hpp"

void generategrid(
    nav2_costmap_2d::Costmap2D * costmap_, 
    std::vector<std::pair<int, int>>& start_points, 
    std::vector<std::pair<int, int>>& goal_points) 
{
    start_points.clear();
    goal_points.clear();

    int c = costmap_->getSizeInCellsX();
    int r = costmap_->getSizeInCellsY();
    
    int botsize = 6; 
    int start_offset = botsize / 2; 

    for (int j = start_offset; j < r; j += botsize) {
        bool left_to_right = ((j / botsize) % 2 == 0);
        int active_start_x = -1; 

        if (left_to_right) {
            if (costmap_->getCost(0, j) == nav2_costmap_2d::FREE_SPACE) {
                active_start_x = 0;
            }
              
            for (int i = 1; i < c; i++) {
                unsigned char current_cost = costmap_->getCost(i, j);
                unsigned char prev_cost = costmap_->getCost(i - 1, j);

                if (current_cost == nav2_costmap_2d::FREE_SPACE && prev_cost == nav2_costmap_2d::LETHAL_OBSTACLE) {
                    active_start_x = i; 
                }
                else if (current_cost == nav2_costmap_2d::LETHAL_OBSTACLE && prev_cost == nav2_costmap_2d::FREE_SPACE) {
                    if (active_start_x != -1) {
                        int goal_x = i - 1; 
                        if (std::abs(goal_x - active_start_x) >= botsize) {
                            start_points.push_back({active_start_x, j});
                            goal_points.push_back({goal_x, j});
                        }
                        active_start_x = -1; 
                    }
                }
            }

            if (active_start_x != -1 && costmap_->getCost(c - 1, j) == nav2_costmap_2d::FREE_SPACE) {
                int goal_x = c - 1;
                if (std::abs(goal_x - active_start_x) >= botsize) {
                    start_points.push_back({active_start_x, j});
                    goal_points.push_back({goal_x, j});
                }
            }

        } else {
            if (costmap_->getCost(c - 1, j) == nav2_costmap_2d::FREE_SPACE) {
                active_start_x = c - 1;
            }
            
            for (int i = c - 2; i >= 0; i--) {
                unsigned char current_cost = costmap_->getCost(i, j);
                unsigned char prev_cost = costmap_->getCost(i + 1, j); 

                if (current_cost == nav2_costmap_2d::FREE_SPACE && prev_cost == nav2_costmap_2d::LETHAL_OBSTACLE) {
                    active_start_x = i; 
                }
                else if (current_cost == nav2_costmap_2d::LETHAL_OBSTACLE && prev_cost == nav2_costmap_2d::FREE_SPACE) {
                    if (active_start_x != -1) {
                        int goal_x = i + 1; 
                        if (std::abs(active_start_x - goal_x) >= botsize) {
                            start_points.push_back({active_start_x, j});
                            goal_points.push_back({goal_x, j});
                        }
                        active_start_x = -1; 
                    }
                }
            }
            
            if (active_start_x != -1 && costmap_->getCost(0, j) == nav2_costmap_2d::FREE_SPACE) {
                int goal_x = 0;
                if (std::abs(active_start_x - goal_x) >= botsize) {
                    start_points.push_back({active_start_x, j});
                    goal_points.push_back({goal_x, j});
                }
            }
        }
    } 
}

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("grid_tester_node");

    RCLCPP_INFO(node->get_logger(), "Initializing Mock Costmap...");

    unsigned int cells_x = 20;
    unsigned int cells_y = 20;
    nav2_costmap_2d::Costmap2D mock_map(
        cells_x, cells_y, 0.05, 0.0, 0.0, nav2_costmap_2d::FREE_SPACE);

    // Build the wall
    for (unsigned int my = 5; my <= 15; ++my) {
        mock_map.setCost(10, my, nav2_costmap_2d::LETHAL_OBSTACLE);
        mock_map.setCost(11, my, nav2_costmap_2d::LETHAL_OBSTACLE);
    }

    RCLCPP_INFO(node->get_logger(), "Map Created. Running generategrid()...");

    std::vector<std::pair<int, int>> starts;
    std::vector<std::pair<int, int>> goals;

    generategrid(&mock_map, starts, goals);

    RCLCPP_INFO(node->get_logger(), "--- GENERATED SEGMENTS ---");
    
    if (starts.size() != goals.size()) {
        RCLCPP_ERROR(node->get_logger(), "CRITICAL: Starts and Goals sizes do not match!");
        return -1;
    }

    for (size_t k = 0; k < starts.size(); ++k) {
        int sx = starts[k].first;
        int sy = starts[k].second;
        int gx = goals[k].first;
        int gy = goals[k].second;
        
        std::cout << "Segment " << k << ": Start(" << sx << ", " << sy 
                  << ") --> Goal(" << gx << ", " << gy << ")" << std::endl;
    }
    
    RCLCPP_INFO(node->get_logger(), "Testing Complete.");
    rclcpp::shutdown();
    return 0;
}