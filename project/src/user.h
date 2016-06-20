#ifndef __USER_HEADER__
#define __USER_HEADER__

#include <string>

class User
{
	private:
	std::string name;
	std::string ip;
	int port;

	public:
	User(const std::string& name, const std::string& ip, int port);
	std::string getName() const;
	int getPort() const;
	std::string getIp() const;
};

#endif
