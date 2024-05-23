

#ifndef SOCKETS_HPP
#define SOCKETS_HPP
#include <sys/socket.h>
#include <netinet/in.h> // sockaddr_in, IPPROTO_TCP
#include <arpa/inet.h> // hton*, ntoh*, inet_addr
#include <unistd.h>  // close
#include <cerrno> // errno
#define SOCKET int
#define INVALID_SOCKET ((int)-1)


class Sockets
{
	public:
		static bool Start()
		{
			return true;
		}
		static int GetError()
		{
			return errno;
		}
		static bool CloseSocket(SOCKET socket)
		{
			return close(socket) == 0;
		}
};
#endif