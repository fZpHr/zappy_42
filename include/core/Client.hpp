#pragma once
#include "../lib.hpp"

namespace zappy {
	namespace core {

		class Client {
			public:
				explicit Client(std::shared_ptr<network::SocketHandler> socket_handler, size_t x, size_t y, zappy::core::Team team);
				void send_message_to(const std::string& message);
				void receive_message_from();
				void disconnect();
				
				size_t get_id() const;
				bool is_connected() const;

				static void initialize_available_ids(size_t max_clients);
			private:
				std::shared_ptr<network::SocketHandler> socket_handler_;
				size_t id_;
				static std::set<size_t> available_ids_;
				size_t x,y;
				zappy::core::Team team;
		};
	}
}