#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <string>

namespace zappy {
	namespace network {

		class SocketHandler {
			public:
				explicit SocketHandler(boost::asio::ip::tcp::socket socket);
				void async_read();
				void async_write(const std::string& message);
				bool is_open() const;
				void close();

			private:
				boost::asio::ip::tcp::socket socket_;
				std::string read_buffer_;
		};

	}
}