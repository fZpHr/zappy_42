#pragma once
#include <string>
#include "../lib.hpp"

class Team {
    public:
        Team(const std::string& name, size_t id, size_t max_size);
        ~Team() = default;
        std::string get_name() const;
        size_t get_x() const;
        size_t get_y() const;
        size_t get_id() const;
        size_t get_max_size() const;
        void reduce_max_size();
        void set_x(size_t x);
        void set_y(size_t y);

    private:
        std::string name_;
        size_t id_;
        size_t max_size_;
        size_t x_;
        size_t y_;
};

