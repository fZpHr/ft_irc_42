#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

#include "Sockets.hpp"

#include <string>
#include <sstream>

class TCPSocket
{
	public:
		TCPSocket();
		~TCPSocket();

		bool Connect(const std::string& ipaddress, unsigned short port);

	private:
		SOCKET mSocket;
};

#endif // TCPSOCKET_HPP