#pragma once
#include "../lib.hpp"

namespace zappy {
	namespace core {

        class CommandHandler {
            public:
                CommandHandler();
                ~CommandHandler() = default;
                void execute(const std::string& command); 
            private:
                std::string current_command_{};
                std::unordered_map<std::string, std::function<void()>> list_commands_{};
        };
	}
}