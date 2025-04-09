#pragma once
#include <utility>
#include <boost/asio.hpp>
#include <memory>
#include <string>
#include "../core/CommandHandler.hpp"
#include "../core/Client.hpp"

class Client;

class SocketHandler : public std::enable_shared_from_this<SocketHandler> {
	public:
		explicit SocketHandler(boost::asio::ip::tcp::socket socket,  std::vector<std::shared_ptr<Team>> &teams)
			: socket_(std::move(socket)), command_handler_(teams) , teams_(teams) {
		}
		void async_read(std::shared_ptr<Client> client);
		void async_write(const std::string &message, const int &id);
		bool is_open() const;
		void close();
		boost::asio::ip::tcp::socket& get_socket();
		virtual ~SocketHandler();
	private:
		boost::asio::ip::tcp::socket socket_;
		std::string read_buffer_;
		CommandHandler command_handler_;
		std::vector<std::shared_ptr<Team>> &teams_;

};
