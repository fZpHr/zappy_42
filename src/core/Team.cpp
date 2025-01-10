#include "../../include/core/Team.hpp"

Team::Team(const std::string &name, size_t x, size_t y, size_t id) : name_(name), x_(x), y_(y), id_(id) {
}


std::string Team::get_name() const {
    return name_;
}

size_t Team::get_id() const {
    return id_;
}

size_t Team::get_x() const {
    return x_;
}

size_t Team::get_y() const {
    return y_;
}




