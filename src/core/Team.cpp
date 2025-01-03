#include "../../include/lib.hpp"

zappy::core::Team::Team(const std::string& name, size_t x, size_t y, size_t id) : name_(name), x(x), y(y), id(id) {}


const std::string& zappy::core::Team::get_name() const {
    return name_;
}

size_t zappy::core::Team::get_id() const {
    return id;
}




