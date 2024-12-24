#pragma once
#include "../lib.hpp"

namespace zappy {
	namespace network {

		class SocketHandler : public std::enable_shared_from_this<SocketHandler> {
			public:
				explicit SocketHandler(boost::asio::ip::tcp::socket socket, size_t id_ = 0);
				void async_read();
				void async_write(const std::string& message);
				bool is_open() const;
				void close();
				boost::asio::ip::tcp::socket& get_socket();

			private:
				boost::asio::ip::tcp::socket socket_;
				std::string read_buffer_;
		};

	}
}