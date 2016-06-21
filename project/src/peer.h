#ifndef __PEER_HEADER__
#define __PEER_HEADER__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <limits>

#define SUCCESS 1
#define MAX_BUF_LEN 4096

enum peer_err: signed int
{
	GETHOSTBYNAME_ERR = std::numeric_limits<int>::min(),
	SOCKET_ERR,
	BIND_ERR,
	SETSOCKOPT_ERR,
	LISTEN_ERR,
	SENDTO_ERR,
	RECVFROM_ERR,
	ACCEPT_ERR,
	NO_CONNECTION_ERR,
	SEND_ERR,
	RECV_ERR,
	CONNECT_ERR
};

class Peer
{
	private:
	struct sockaddr_in addr;
	int sock;
	int bind();	
	std::string received_msg;
	std::string ip_sender;
	unsigned short port_sender;

	protected:
	void setSenderAddr(const struct sockaddr_in& src);
	void setSenderMsg(const std::string& msg);
	
	public:
	Peer();
	~Peer();
	int setup(const std::string& ip, int port, int type, 
		int family=AF_INET);
	int getSock() const;
	unsigned short getPort() const;
	std::string getIp() const;	
	short getFamily() const;
	std::string getReceivedMsg();
	std::string getSenderIp();
	unsigned short getSenderPort();
};

class UDPPeer: public Peer
{
	public:
	UDPPeer();
	int setup(const std::string& ip, int port=-1, int family=AF_INET);
	int send(const struct sockaddr_in& dst, const std::string& msg);
	int send(const std::string& ip, unsigned short port, 
		const std::string& msg);
	int recv(bool blocking=true);
};

class TCPPeer: public Peer
{
	private:
	int conn_sock;
	bool _accept;	

	public:
	TCPPeer();
	~TCPPeer();
	int setup(const std::string& ip, int port=-1, int family=AF_INET);
	int listen(int max_pending=5);
	int connect(const std::string& ip, unsigned short port);
	int accept();
	int send(const std::string& msg);
	int recv(bool blocking=true);
};

#endif
