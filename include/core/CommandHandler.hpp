#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include "../lib.hpp"
#include "Team.hpp"

class CommandHandler {
    public:
        CommandHandler(std::vector<std::shared_ptr<Team>> &teams);
        ~CommandHandler() = default;
        int check_teams(const std::string& command);
        int execute(const std::string& command); 
    private:
        std::string current_command_{};
        std::unordered_map<std::string, std::function<void()>> list_commands_{};
        std::vector<std::shared_ptr<Team>> &teams_;
};
