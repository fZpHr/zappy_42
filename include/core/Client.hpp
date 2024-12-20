#pragma once
#include "../network/SocketHandler.hpp"
#include <memory>
#include <string>

namespace zappy {
	namespace core {

		class Client {
			public:
				explicit Client(std::shared_ptr<network::SocketHandler> socket_handler);
				void send_message(const std::string& message);
				bool is_connected() const;
				size_t get_id() const;

			private:
				std::shared_ptr<network::SocketHandler> socket_handler_;
				size_t id_;
				static size_t next_id_;
		};

	}
}