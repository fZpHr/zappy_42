#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Client.hpp"

class Map {
    public:
        Map(size_t width, size_t height, std::vector<std::shared_ptr<Client>>& clients, std::vector<std::shared_ptr<Team>>& teams);
        ~Map() = default;
        void generateMap();
        void placeTeams();
        void placeResources();
        void printMap(std::ostream &os);
        std::string serialize() const;
    private:
        size_t width_;
        size_t height_;
        std::vector<std::vector<std::string>> map_;
        std::vector<std::shared_ptr<Client>> &clients_;
        std::vector<std::shared_ptr<Team>> &teams_;
};
