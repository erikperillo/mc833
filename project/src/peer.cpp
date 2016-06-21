#include "peer.h"
#include <cstring>

int fillSockAddrIn(struct sockaddr_in& addr, const std::string& ip, 
	int port, int type, int family)
{
	struct hostent* hp;

	hp = gethostbyname(ip.c_str());
	if(!hp)
		return GETHOSTBYNAME_ERR;

	memset((char*)&addr, 0, sizeof(addr));	
	addr.sin_family = family;
	if(port >= 0)
		addr.sin_port = htons((unsigned short)port);
	memcpy((char*)&addr.sin_addr, hp->h_addr, hp->h_length);

	return SUCCESS;
}

int Peer::setup(const std::string& ip, int port, int type, int family)
{
	int opt;
	int ret;

	ret = fillSockAddrIn(this->addr, ip, port, type, family);
	if(ret != SUCCESS)
		return ret;

	this->sock = socket(family, type, 0);
	if(this->sock < 0)
		return SOCKET_ERR;

	opt = 1;
	ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, 
		sizeof(int));
	if(ret < 0)
		return SETSOCKOPT_ERR;

	if(port >= 0)
	{
		ret = this->bind();
		if(ret < 0)
			return BIND_ERR;
	}

	return SUCCESS;
}

int Peer::bind()
{
	int ret;

	ret = ::bind(this->sock, (struct sockaddr*)&this->addr, sizeof(this->addr));

	return (ret < 0)?BIND_ERR:SUCCESS;
}
/*
int Peer::listen(int max_pending)
{
	int ret;

	ret = ::listen(this->sock, max_pending);

	return (ret < 0)?LISTEN_ERR:SUCCESS;
}*/

int Peer::getSock() const
{
	return this->sock;
}

unsigned short Peer::getPort() const
{
	return ntohs(this->addr.sin_port);
}

std::string Peer::getIp() const
{
	return std::string(inet_ntoa(this->addr.sin_addr));
}

short Peer::getFamily() const
{
	return this->addr.sin_family;
}

std::string Peer::getReceivedMsg()
{
	return this->received_msg;
}

std::string Peer::getSenderIp()
{
	return this->ip_sender;
}

unsigned short Peer::getSenderPort()
{
	return this->port_sender;
}

void Peer::setSenderAddr(const struct sockaddr_in& src)
{
	this->ip_sender = std::string(inet_ntoa(src.sin_addr));
	this->port_sender = ntohs(src.sin_port);
}

void Peer::setSenderMsg(const std::string& msg)
{
	this->received_msg = msg;
}

Peer::Peer()
{;}

Peer::~Peer()
{
	close(this->sock);
}

UDPPeer::UDPPeer()
{;}

int UDPPeer::setup(const std::string& ip, int port, int family)
{
	return Peer::setup(ip, port, SOCK_DGRAM, family);
}

int UDPPeer::send(const struct sockaddr_in& dst, const std::string& msg)
{
	int ret;

	ret = sendto(this->getSock(), msg.c_str(), msg.size(), 0, 
		(struct sockaddr*)&dst, sizeof(dst));
	if(ret < 0)
		return SENDTO_ERR;

	return SUCCESS;
}
/*
int UDPPeer::send(const Peer& peer, const std::string& msg)
{
	struct sockaddr_in dst;
	int ret;

	ret = fillSockAddrIn(dst, peer.getIp(), peer.getPort(), SOCK_DGRAM,
		peer.getFamily());
	if(ret != SUCCESS)
		return ret;

	return this->send(dst, msg);
}*/

int UDPPeer::send(const std::string& ip, unsigned short port, 
	const std::string& msg)
{
	struct sockaddr_in dst;
	int ret;

	ret = fillSockAddrIn(dst, ip, port, SOCK_DGRAM, this->getFamily());
	if(ret != SUCCESS)
		return ret;

	return this->send(dst, msg);
}

int UDPPeer::recv(bool blocking)
{
	struct sockaddr_in src;
	socklen_t addrlen;
	char buf[MAX_BUF_LEN] = "\0";
	int ret;

	addrlen = sizeof(src);
	ret = recvfrom(this->getSock(), buf, MAX_BUF_LEN, blocking?0:MSG_DONTWAIT,
		(struct sockaddr*)&src, &addrlen);
	if(ret < 0)
		return RECVFROM_ERR;

	this->setSenderAddr(src);
	this->setSenderMsg(std::string(buf));

	return ret;
}


TCPPeer::TCPPeer(): conn_sock(-1), _accept(false)
{;}

int TCPPeer::setup(const std::string& ip, int port, int family)
{
	return Peer::setup(ip, port, SOCK_STREAM, family);
}

int TCPPeer::listen(int max_pending)
{
	int ret;

	ret = ::listen(this->getSock(), max_pending);

	return (ret < 0)?LISTEN_ERR:SUCCESS;
}

int TCPPeer::connect(const std::string& ip, unsigned short port)
{
	struct sockaddr_in dst;
	int ret;

	ret = fillSockAddrIn(dst, ip, port, SOCK_STREAM, this->getFamily());
	if(ret != SUCCESS)
		return ret;

	ret = ::connect(this->getSock(), (struct sockaddr*)&dst, sizeof(dst));
	if(ret < 0)
		return CONNECT_ERR;

	this->setSenderAddr(dst);

	return SUCCESS;	
}

int TCPPeer::accept()
{
	struct sockaddr_in dst;
	socklen_t len;

	len = sizeof(sockaddr_in);
	this->conn_sock = ::accept(this->getSock(), (struct sockaddr*)&dst, &len);
	if(this->conn_sock < 0)
		return ACCEPT_ERR;

	this->setSenderAddr(dst);
	this->_accept = true;

	return SUCCESS;	
}

int TCPPeer::send(const std::string& msg)
{
	int ret;

	ret = ::send(this->_accept?this->conn_sock:this->getSock(), 
		msg.c_str(), msg.size(), 0);
	if(ret < 0)
		return SEND_ERR;

	return ret;
}

int TCPPeer::recv(bool blocking)
{
	int ret;
	char buf[MAX_BUF_LEN] = "\0";

	if(this->_accept && this->conn_sock < 0)
		return NO_CONNECTION_ERR;

	ret = ::recv(this->_accept?this->conn_sock:this->getSock(), 
		buf, MAX_BUF_LEN, blocking?0:MSG_DONTWAIT);
	if(ret < 0)
		return RECV_ERR;

	this->setSenderMsg(std::string(buf));

	return ret;
}

TCPPeer::~TCPPeer()
{
	if(this->conn_sock >= 0)
		close(this->conn_sock);
}
