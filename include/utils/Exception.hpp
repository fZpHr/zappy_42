#pragma once
#include <stdexcept>
#include <string>

class ZappyException : public std::runtime_error {
	public:
		explicit ZappyException(const std::string& message) : std::runtime_error(message) {}
};

class NetworkException : public ZappyException {
	public:
		explicit NetworkException(const std::string& message) : ZappyException(message) {}
};
