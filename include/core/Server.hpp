#pragma once
#include "../lib.hpp"

namespace zappy {
    namespace core {

        class Server {
            public:
                explicit Server(int port);
                virtual ~Server() = default;

                void start();
                void stop();
                bool is_running() const;
                size_t get_client_count() const;
                static void signal_handler(int signal);

            private:

                std::unique_ptr<network::NetworkManager> network_manager_;
                std::vector<std::shared_ptr<Client>> clients_;
                bool running_;
                static std::atomic<bool> signal_received;
        };

    }
}
