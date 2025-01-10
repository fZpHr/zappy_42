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

CommandHandler::CommandHandler() {
   for (const auto& command : COMMANDS) {
       list_commands_[command.first] = [msg = command.second]() { INFO(msg); };
   }
}

void CommandHandler::execute(const std::string& command) {
   auto it = list_commands_.find(command);
   if (it != list_commands_.end()) {
       it->second();
   }
}