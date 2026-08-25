
#include<vector>
#include<cmath>
#include<utility>
#include "nav_msgs/msg/occupancy_grid.hpp"

#include "coverage_commander/row_scanner.hpp"


void generategrid(const nav_msgs::msg::OccupancyGrid& grid, std::vector<std::pair<int, int>>& start_p, std::vector<std::pair<int, int>>& goal_p, int footprint){

int c = grid.info.width;
int r = grid.info.height; //option to follow whichever is larger.
int botsize = footprint;

    for(int j = botsize/2; j<r; j+=botsize){
        std::pair<int, int> temp_s;
        std::pair<int, int> temp_g;
    
        if((j/botsize)%2 == 0){

            if(grid.data[j*c] == 0){
                temp_s.first = 0;
                temp_s.second = j;
            }
                
            for(int i = 1; i<c; i++){
                
                if((grid.data[j*c + i] == 100 || grid.data[j*c + i] == -1) && grid.data[j*c + i-1] == 0){
                    temp_g.first = i-1;
                    temp_g.second = j;

                    if(std::abs(temp_s.first - temp_g.first) >= botsize){
                        start_p.emplace_back(temp_s);
                        goal_p.emplace_back(temp_g);
                    }
                    // goal_p.push_back({i-1, j});
                }

                else if(grid.data[j*c + i] == 0 && (grid.data[j*c + i-1] == 100 || grid.data[j*c + i-1] == -1)){
                    temp_s.first = i;
                    temp_s.second = j;   
                }


            }
            
            if(grid.data[j*c + c-1] == 0){
                temp_g.first = c-1; 
                temp_g.second = j;

                if(std::abs(temp_s.first - temp_g.first) >= botsize){
                    start_p.emplace_back(temp_s);
                    goal_p.emplace_back(temp_g);
                }

            }
        }

        else{
        
            if(grid.data[j*c + c-1] == 0){
                temp_s.first = c-1;
                temp_s.second = j;                
            }

            for(int i = c-2; i>=0; i--){

                if((grid.data[j*c + i] == 100 || grid.data[j*c + i] == -1) && grid.data[j*c + i+1] == 0){
                    temp_g.first = i+1;
                    temp_g.second = j;                    
                    

                    if(std::abs(temp_s.first - temp_g.first) >= botsize){
                        start_p.emplace_back(temp_s);
                        goal_p.emplace_back(temp_g);
                    }
                
                }

                else if(grid.data[j*c + i] == 0 && (grid.data[j*c + i+1] == 100 || grid.data[j*c + i+1] == -1)){
                    temp_s.first = i;
                    temp_s.second = j;
                    
                }
                
            }

            if(grid.data[j*c] == 0){
                temp_g.first = 0;
                temp_g.second = j;

                if(std::abs(temp_s.first - temp_g.first) >= botsize){
                    start_p.emplace_back(temp_s);
                    goal_p.emplace_back(temp_g);
                }            
            }

        }

    }    
} 

