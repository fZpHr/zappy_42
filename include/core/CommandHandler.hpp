#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include "../lib.hpp"

class CommandHandler {
    public:
        CommandHandler();
        ~CommandHandler() = default;
        void execute(const std::string& command); 
    private:
        std::string current_command_{};
        std::unordered_map<std::string, std::function<void()>> list_commands_{};
};
