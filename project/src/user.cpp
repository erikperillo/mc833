#include "user.h"

User::User(const std::string& name, const std::string& ip, int port):
	name(name), ip(ip), port(port)
{;}

std::string User::getName() const
{
	return this->name;
}

int User::getPort() const
{
	return this->port;
}

std::string User::getIp() const
{
	return this->ip;
}
