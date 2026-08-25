#ifndef COVERAGE_COMMANDER__ROW_SCANNER_HPP_
#define COVERAGE_COMMANDER__ROW_SCANNER_HPP_

#include <vector>
#include <utility>
#include "nav_msgs/msg/occupancy_grid.hpp"

void generategrid(
  const nav_msgs::msg::OccupancyGrid & grid,
  std::vector<std::pair<int, int>> & start_p,
  std::vector<std::pair<int, int>> & goal_p);

#endif