#include "../../include/core/Team.hpp"

Team::Team(const std::string &name, size_t id, size_t max_size) : name_(name), id_(id), max_size_(max_size) {
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

size_t Team::get_max_size() const {
    return max_size_;
}

void Team::reduce_max_size() {
    if (max_size_ > 0) {
        max_size_--;
    }
    else {
        ERROR("Max size of team " + name_ + " is already 0");
    }
}


