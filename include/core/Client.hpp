#pragma once
#include "../lib.hpp"

namespace zappy {
	namespace core {

		class Client {
			public:
				explicit Client(std::shared_ptr<network::SocketHandler> socket_handler);
				void send_message_to(const std::string& message);
				void receive_message_from();
				bool is_connected() const;
				void disconnect();
				
				size_t get_id() const;

				static void initialize_available_ids(size_t max_clients);
			private:
				std::shared_ptr<network::SocketHandler> socket_handler_;
				size_t id_;
				static std::set<size_t> available_ids_;
		};
	}
}