#include "../../include/core/CommandHandler.hpp"


const std::unordered_map<std::string, std::string> COMMANDS = {
    {"Forward", "Forward"},
    {"Right", "Right"},
    {"Left", "Left"},
    {"Look", "Look"},
    {"Inventory", "Inventory"},
    {"Broadcast", "Broadcast"},
    {"Connect_nbr", "Connect_nbr"},
    {"Fork", "Fork"},
    {"Eject", "Eject"},
    {"Take", "Take"}, 
    {"Set", "Set"},
    {"Incantation", "Incantation"}
};

CommandHandler::CommandHandler(std::vector<std::shared_ptr<Team>>& teams)
    : teams_(teams) {
   for (const auto& command : COMMANDS) {
       list_commands_[command.first] = [msg = command.second]() { INFO(msg); };
   }
}

int CommandHandler::check_teams(const std::string& command) {
    for (auto &team : teams_) {
        if (team->get_name() == command && team->get_max_size() > 0) {
            team->reduce_max_size();
            return 1;
        }
        else if (team->get_name() == command && team->get_max_size() <= 0) {
            ERROR("Team " + command + " is full");
            return 0;
        }
    }
    return 0;
}
    
int CommandHandler::execute(const std::string& command) {
    auto it = list_commands_.find(command);
    if (it != list_commands_.end()) {
        it->second();
        return 0;
    }
    else if (check_teams(command)) {
        return 2;
    }
    else {
        ERROR("Unknown command: " + command);
        return 1;
    }
}