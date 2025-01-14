#include "../../include/core/Team.hpp"

Team::Team(const std::string &name, size_t id) : name_(name), id_(id) {
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

void Team::set_x(size_t x) {
    x_ = x;
}

void Team::set_y(size_t y) {
    y_ = y;
}



