#include "../../include/core/Map.hpp"

Map::Map(size_t width, size_t height, std::vector<std::shared_ptr<Client>> &clients, std::vector<std::shared_ptr<Team>> &teams) : width_(width), height_(height), clients_(clients), teams_(teams) {
    generateMap(); 
}

void Map::printMap(std::ostream &os) {
    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < width_; j++) {
            os << map_[i][j] << " ";
        }
        os << std::endl;
    }
}

void Map::placeTeams() {
    DEBUG("Placing teams on the map...");
    std::vector<std::pair<int, int>> positions;
    DEBUG(std::to_string(teams_.size()).c_str());
    for (size_t i = 0; i < teams_.size(); i++) {
        DEBUG("Placing team " + std::to_string(i + 1) + "...");
        int x, y;

        do {
            x = rand() % width_;
            y = rand() % height_;
        } while (map_[y][x] != "0");

        DEBUG("Random position for team " + std::to_string(i + 1) + ": (" + std::to_string(x) + ", " + std::to_string(y) + ")");
        map_[y][x] = std::to_string(i + 1);
        teams_[i]->set_x(x);
        teams_[i]->set_y(y);
    }
}

void Map::placeResources() {
    std::vector<std::pair<int, int>> positions;
    
    for (int i = 0; i < 10; i++) {
        int x = rand() % width_;
        int y = rand() % height_;
        
        if (map_[y][x] == "0") {
            map_[y][x] = "F";
        }
    }
}

void Map::generateMap() {
    map_.resize(height_);
    for (size_t i = 0; i < height_; i++) {
        map_[i].resize(width_);
        for (size_t j = 0; j < width_; j++) {
            map_[i][j] = "0";
        }
    }
    placeResources();
}

