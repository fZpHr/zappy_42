#include "../../include/lib.hpp"

zappy::core::Map::Map(size_t width, size_t height) : width_(width), height_(height) {
    generate_map();
}

void zappy::core::Map::generate_map() {
    map_.resize(height_);
    for (size_t i = 0; i < height_; i++) {
        map_[i].resize(width_);

    }
}

void zappy::core::Map::print_map() {
    std::cout << "-----------------" << std::endl;
    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < width_; j++) {
            std::cout << map_[i][j] << "#";
        }
        std::cout << std::endl;
    }   
    std::cout << "-----------------" << std::endl;
}


