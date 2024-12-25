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

			private:
				std::shared_ptr<network::SocketHandler> socket_handler_;
				size_t id_;
				static size_t next_id_;
		};

	}
}