#include "peer.h"
#include <cstring>

int Peer::setup(const std::string& ip, const unsigned short port,
	int type, int family)
{
	struct hostent* hp;
	int opt;
	int ret;

	hp = gethostbyname(ip.c_str());
	if(!hp)
		return GETHOSTBYNAME_ERR;

	memset((char*)&this->addr, 0, sizeof(this->addr));	
	this->addr.sin_family = family;
	this->addr.sin_port = htons(port);
	memcpy((char*)&this->addr.sin_addr, hp->h_addr, hp->h_length);

	this->sock = socket(family, type, 0);
	if(this->sock < 0)
		return SOCKET_ERR;

	opt = 1;
	ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, 
		sizeof(int));
	if(ret < 0)
		return SETSOCKOPT_ERR;

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

int Peer::getSock()
{
	return this->sock;
}

unsigned short Peer::getPort()
{
	return ntohs(this->addr.sin_port);
}

std::string Peer::getIp()
{
	char* str_ip;

	str_ip = inet_ntoa(this->addr.sin_addr);

	return std::string(str_ip);
}

Peer::Peer()
{;}
