#pragma once
#include "Client.hpp"
#include "../network/NetworkManager.hpp"
#include <memory>
#include <vector>

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

            private:
                std::unique_ptr<network::NetworkManager> network_manager_;
                std::vector<std::shared_ptr<Client>> clients_;
                bool running_;
        };

    }
}
