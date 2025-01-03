#pragma once
#include "../lib.hpp"

namespace zappy {
    namespace core {
        
        class Map {
            public:
                Map(size_t width, size_t height);
                ~Map() = default;
                void generate_map();
                void print_map();
            private:
                size_t width_;
                size_t height_;
                std::vector<std::vector<std::string>> map_;
                std::vector<std::shared_ptr<Client>> clients_;
        };
    }
}


