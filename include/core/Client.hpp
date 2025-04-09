#pragma once
#include <set>
#include <utility>
#include <string>	
#include <memory>
#include "Team.hpp"
#include "../network/SocketHandler.hpp"

class SocketHandler; 

class NetworkManager;

class Client : public std::enable_shared_from_this<Client> {
	public:
		Client(std::shared_ptr<SocketHandler> socket_handler, std::vector<std::shared_ptr<Team>>& teams);
		void send_message_to(const std::string& message);
		void receive_message_from();
		void disconnect();
		
		size_t get_id() const;
		string get_team_name() const;
		void set_team(Team team);
		bool is_connected() const;

		static void initialize_available_ids(size_t max_clients);
	private:
		std::shared_ptr<SocketHandler> socket_handler_;
		size_t id_;
		static std::set<size_t> available_ids_;
		size_t x,y;
		string team_name;
		std::vector<std::shared_ptr<Team>>& teams_;
};