#include "../../include/core/Map.hpp"

Map::Map(size_t width, size_t height, std::vector<std::shared_ptr<Client>> &clients, std::vector<std::shared_ptr<Team>> &teams) : width_(width), height_(height), clients_(clients), teams_(teams) {
    // generateMap();
}

void Map::printMap() {
    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < width_; j++) {
            std::cout << map_[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Map::placeTeams() {
    std::vector<std::pair<int, int>> positions;
    
    for (size_t i = 0; i < teams_.size(); i++) {
        int best_x = 0, best_y = 0;
        double max_min_distance = 0;
        
        for (size_t x = 0; x < width_; x++) {
            for (size_t y = 0; y < height_; y++) {
                double min_distance = std::numeric_limits<double>::max();
                
                for (const auto& pos : positions) {
                    double dist = std::sqrt(std::pow(x - pos.first, 2) + 
                                          std::pow(y - pos.second, 2));
                    min_distance = std::min(min_distance, dist);
                }
                
                if (min_distance > max_min_distance) {
                    max_min_distance = min_distance;
                    best_x = x;
                    best_y = y;
                }
            }
        }
        
        positions.push_back({best_x, best_y});
        teams_[i]->set_x(best_x);
        teams_[i]->set_y(best_y);
    }
}

void Map::placeResources() {
    std::vector<std::pair<int, int>> positions;
    
    for (int i = 0; i < 10; i++) {
        int x = rand() % width_;
        int y = rand() % height_;
        
        if (map_[y][x] == "empty") {
            map_[y][x] = "food";
        }
    }
}

void Map::generateMap() {
    map_.resize(height_);
    for (size_t i = 0; i < height_; i++) {
        map_[i].resize(width_);
        for (size_t j = 0; j < width_; j++) {
            map_[i][j] = "empty";
        }
    }
    
    placeTeams();
    placeResources();
}

