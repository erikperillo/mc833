#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include "net.h"
#include "protocol.h"

using namespace std;

#define SERVER_NAME "127.0.0.1"
#define SERVER_PORT 7532

void error(const std::string& msg, int ret=1)
{
	perror(msg.c_str());
	exit(ret);
}

void client()
{
	NetAddr server(SERVER_NAME, SERVER_PORT);	
	int sock;
	int ret;
	NetMessage msg;
	NetAddr conn;
	std::string str;
	
	if(server.getErrCode() < 0)
		error("NetAddr");

	sock = getSocket(SOCK_STREAM);	
	if(sock < 0)
		error("getSocket");

	if(connect(sock, server) < 0)
		error("connect");	

	cout << "connected: " << server.getIp() << ":" << server.getPort() << endl;

	while(true)
	{
		cout << ">>> ";
		cin >> str;

		ret = send(sock, str);
		if(ret < 0)
			error("send");	
		if(ret == 0)
			error("connection ended", 0);
	}
}

int main()
{
	client();
	return 0;
}
