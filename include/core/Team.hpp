#pragma once
#include <string>

class Team {
    public:
        Team(const std::string& name, size_t id);
        ~Team() = default;
        std::string get_name() const;
        size_t get_x() const;
        size_t get_y() const;
        size_t get_id() const;
        void set_x(size_t x);
        void set_y(size_t y);
    private:
        std::string name_;
        size_t x_;
        size_t y_;
        size_t id_;
};

