#include "coverage_commander/coverage_node.hpp"

int main(int argc, char*argv[]){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<CoverageNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}