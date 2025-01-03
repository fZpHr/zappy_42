#pragma once
#include "../lib.hpp"

namespace zappy {
    namespace core {

        class Server {
            public:
                explicit Server(boost::program_options::variables_map &vm);
                virtual ~Server() = default;

                void start();
                void stop();
                bool is_running() const;
                size_t get_client_count() const;
                static void signal_handler(int signal);

            private:
                std::unique_ptr<zappy::network::NetworkManager> network_manager_;
                std::vector<std::shared_ptr<Client>> clients_;
                std::set<size_t> available_ids_;
                bool running_ = false;
                static std::atomic<bool> signal_received;
                const boost::program_options::variables_map settings_;
                Map map_;
        };

    }
}
