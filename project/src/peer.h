#ifndef __PEER_HEADER__
#define __PEER_HEADER__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <limits>

enum err: signed int
{
	GETHOSTBYNAME_ERR = std::numeric_limits<int>::min(),
	SOCKET_ERR,
	BIND_ERR,
	SETSOCKOPT_ERR,
	LISTEN_ERR
};

#define SUCCESS 1

class Peer
{
	private:
	struct sockaddr_in addr;
	int sock;

	public:
	Peer();
	int setup(const std::string& ip, const unsigned short port, int type, 
		int family=AF_INET);
	int bind();	
	int getSock();
	unsigned short getPort();
	std::string getIp();	
};

#endif
