#pragma once
#include "../lib.hpp"

namespace zappy {
    namespace core {

        class Team {
            public:
                Team(const std::string& name, size_t x, size_t y, size_t id);
                ~Team() = default;
                const std::string& get_name() const;
                size_t get_id() const;
            private:
                std::string name_;
                size_t x;
                size_t y;
                size_t id;
        };
    }
}

