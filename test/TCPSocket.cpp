#include "TCPSocket.hpp"


TCPSocket::~TCPSocket()
{
	Sockets::CloseSocket(mSocket);
}

TCPSocket::TCPSocket()
{
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSocket == INVALID_SOCKET)
	{
		std::stringstream error;
		error << "Erreur initialisation socket [" << Sockets::GetError() << "]";
		throw std::runtime_error(error.str());
	}
}
bool TCPSocket::Connect(const std::string& ipaddress, unsigned short port)
{
	sockaddr_in server;
	server.sin_addr.s_addr = inet_addr(ipaddress.c_str());
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	return connect(mSocket, reinterpret_cast<const sockaddr*>(&server), sizeof(server)) == 0;
}