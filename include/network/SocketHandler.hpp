#pragma once
#include <utility>
#include <boost/asio.hpp>
#include <memory>
#include <string>
#include "../core/CommandHandler.hpp"

class SocketHandler : public std::enable_shared_from_this<SocketHandler> {
	public:
		explicit SocketHandler(boost::asio::ip::tcp::socket socket);
		void async_read(const size_t &id);
		void async_write(const std::string &message, const int &id);
		bool is_open() const;
		void close();
		boost::asio::ip::tcp::socket& get_socket();
		virtual ~SocketHandler();
	private:
		boost::asio::ip::tcp::socket socket_;
		std::string read_buffer_ {};
		CommandHandler command_handler_ {};
};
