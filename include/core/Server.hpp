#pragma once

#include "../lib.hpp"
#include <boost/program_options.hpp>
#include <memory>
#include <vector>
#include <set>
#include <atomic>
#include "../network/NetworkManager.hpp"
#include "Client.hpp"
#include "Map.hpp"
#include <thread>
#include <fstream>

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
        std::vector<std::shared_ptr<Client>> clients_;
        std::vector<std::shared_ptr<Team>> teams_;
        std::set<size_t> available_ids_;
        bool running_ = false;
        static std::atomic<bool> signal_received;
        const boost::program_options::variables_map settings_;
        Map map_;
        std::unique_ptr<NetworkManager> network_manager_;
        
};
