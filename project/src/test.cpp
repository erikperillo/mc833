#include <iostream>
#include <string>
#include "chat.h"
#include "user.h"
#include "group.h"
#include "message.h"
#include "chatview.h"
#include "net.h"
#include "protocol.h"

using namespace std;

inline void addUserCheck(Chat& chat, const string& name)
{
	if(!chat.addUser(User(name, "127.0.0.1", 0)))
		cout << "user '" << name << "' could not be added to chat" << endl;
	else
		cout << "user '" << name << "' added to chat" << endl;
}

inline void addGroupCheck(Chat& chat, const string& name)
{
	if(!chat.addGroup(name))
		cout << "group '" << name << "' could not be added to chat" << endl;
	else
		cout << "group '" << name << "' added to chat" << endl;
}

inline void addUserToGroupCheck(Chat& chat, const string& user_name,
	const string& group_name)
{
	if(!chat.addUserToGroup(user_name, group_name))
		cout << "user '" << user_name << "' could not be added to group '" << 
		group_name << "'" << endl;
	else
		cout << "user '" << user_name << "' added to group '" << 
		group_name << "'" << endl;
}


inline void addMessageCheck(Chat& chat, const Message& msg)
{
	if(!chat.addMessage(msg))
		cout << "message #" << msg.getId() << 
		" could not be added to chat" << endl;
	else
		cout << "message #" << msg.getId() << " added to chat" << endl;
}

void error(const std::string& msg, int ret_code=1)
{
	cout << "error: " << msg << endl;
	exit(ret_code);
}

void chatTest()
{
	Chat chat;

	addUserCheck(chat, "erik");
	addUserCheck(chat, "larissa");
	addUserCheck(chat, "erik");
	addUserCheck(chat, "mario");
	addUserCheck(chat, "jose");
	addUserCheck(chat, "leandro");

	addGroupCheck(chat, "safados");
	addGroupCheck(chat, "safados");
	addGroupCheck(chat, "pilantras");
	addGroupCheck(chat, "safados");
	addGroupCheck(chat, "gostosos");
	addGroupCheck(chat, "gostosos");

	Message msg1 = Message("", "", "");
	Message msg2 = Message("", "", "");
	addMessageCheck(chat, msg1);
	addMessageCheck(chat, msg1);
	addMessageCheck(chat, msg2);
	addMessageCheck(chat, msg2);
	addMessageCheck(chat, msg1);

	addUserToGroupCheck(chat, "erik", "safados");
	addUserToGroupCheck(chat, "erik", "pilantras");
	addUserToGroupCheck(chat, "larissa", "pilantras");
	addUserToGroupCheck(chat, "erik", "pilantras");
	addUserToGroupCheck(chat, "erik", "kajdhf");
	addUserToGroupCheck(chat, "alkjdfh", "pilantras");
}

void chatViewTest()
{
	Chat chat;
	ChatView view(chat);

	addUserCheck(chat, "erik");
	addUserCheck(chat, "larissa");
	addUserCheck(chat, "mario");
	addUserCheck(chat, "jose");
	addUserCheck(chat, "leandro");

	addGroupCheck(chat, "safados");
	addGroupCheck(chat, "pilantras");
	addGroupCheck(chat, "safados");
	addGroupCheck(chat, "gostosos");

	Message msg1 = Message("", "", "");
	Message msg2 = Message("", "", "");
	addMessageCheck(chat, msg1);
	addMessageCheck(chat, msg2);

	addUserToGroupCheck(chat, "erik", "safados");
	addUserToGroupCheck(chat, "erik", "pilantras");
	addUserToGroupCheck(chat, "larissa", "pilantras");

	cout << "users:" << endl;
	view.printUsers();

	cout << "groups:" << endl;
	view.printGroups();

	view.printUsersFromGroup("pilantras");
}

#define SERVER_PORT 7532

void UDPPeerTestServer()
{
	NetAddr addr("127.0.0.1", SERVER_PORT);	
	int sock;
	NetMessage msg;
	NetAddr src;
	
	if(addr.getErrCode() < 0)
		error("NetAddr");

	sock = getSocket(SOCK_DGRAM);	
	if(sock < 0)
		error("getSocket");

	if(bind(sock, addr) < 0)
	{
		perror("bind");
		error("bind");
	}

	while(true)
	{
		msg = recvFrom(sock);
		if(msg.getErrCode() < 0)
			error("recvFrom");	

		src = msg.getSrcAddr();
		cout << "[" << src.getIp() << ":" << src.getPort() << "] "
			<< msg.getContent() << endl;

		if(sendTo(sock, src, msg.getContent()) < 0)
			error("sendTo");
	}
}

void TCPPeerTestServer()
{
	NetAddr addr("127.0.0.1", SERVER_PORT);	
	int sock;
	int new_sock;
	NetMessage msg;
	NetAddr conn;
	
	if(addr.getErrCode() < 0)
		error("NetAddr");

	sock = getSocket(SOCK_STREAM);	
	if(sock < 0)
		error("getSocket");

	if(bind(sock, addr) < 0)
	{
		perror("bind");
		error("bind");
	}

	if(attend(sock) < 0)
		error("listen");

	conn = accept(sock);	
	new_sock = conn.getErrCode();
	if(new_sock < 0)
	{
		perror("accept");
		error("accept");
	}

	cout << "new connection on " << conn.getIp() << ":" << conn.getPort() 
		<< endl;

	while(true)
	{
		msg = recv(new_sock);
		if(msg.getErrCode() < 0)
			error("recv");	
		if(msg.getErrCode() == 0)
			error("connection ended.");	

		cout << msg.getContent() << endl;

		if(send(new_sock, msg.getContent()) < 0)
			error("send");
	}
}

void UDPPeerTestClient()
{
	NetAddr addr("127.0.0.1", SERVER_PORT+1);	
	NetAddr dst("127.0.0.1", SERVER_PORT);
	NetAddr src;
	int sock;
	NetMessage msg;
	std::string str;
	
	if(addr.getErrCode() < 0)
		error("NetAddr1");
	if(dst.getErrCode() < 0)
		error("NetAddr2");

	sock = getSocket(SOCK_DGRAM);	
	if(sock < 0)
		error("getSocket");

	while(true)
	{
		cout << ">>> ";
		cin >> str;

		//NetMessage message(addr, dst, str);
		//if(sendTo(sock, message) < 0)
		if(sendTo(sock, dst, str) < 0)
			error("sendTo");	

		msg = recvFrom(sock);
		if(msg.getErrCode() < 0)
			error("recvFrom");

		src = msg.getSrcAddr();
		cout << "[" << src.getIp() << ":" << src.getPort() << "] "
			<< msg.getContent() << endl;
	}
}

void TCPPeerTestClient()
{
	NetAddr dst("127.0.0.1", SERVER_PORT);	
	int sock;
	int ret;
	NetMessage msg;
	NetAddr conn;
	std::string str;
	
	if(dst.getErrCode() < 0)
		error("NetAddr");

	sock = getSocket(SOCK_STREAM);	
	if(sock < 0)
		error("getSocket");

	if(connect(sock, dst) < 0)
		error("connect");	

	cout << "connected to " << dst.getIp() << ":" << dst.getPort() << endl;

	while(true)
	{
		cout << ">>> ";
		cin >> str;

		ret = send(sock, str);
		if(ret < 0)
			error("send");	

		msg = recv(sock);
		if(msg.getErrCode() < 0)
			error("recv");

		cout << msg.getContent() << endl;
	}
}

void protocolTest()
{
	string str = ":oie::::::::::::td: :bem";
		
	cout << "original: " << str << endl;
	cout << "sanitize: " << sanitize(str) << endl;
	cout << "desanitize: " << desanitize(sanitize(str)) << endl;

	vector<string> tokens = split(str, ":");
	for(auto const& t: tokens)
		cout << t << endl;

	Message _msg("e:rik", "tai::na:", "::::e ai: OTARIA:");
	string msg = hostToNetMsg(_msg);
	cout << msg << endl;
	for(auto const& t: split(msg, ":"))
		cout << desanitize(t) << endl;
}

int main()
{
	#ifdef MODEL
	chatTest();
	#endif
	#ifdef VIEW
	chatViewTest();
	#endif
	#ifdef UDPPEERSERVER
	UDPPeerTestServer();
	#endif
	#ifdef UDPPEERCLIENT
	UDPPeerTestClient();
	#endif
	#ifdef TCPPEERSERVER
	TCPPeerTestServer();
	#endif
	#ifdef TCPPEERCLIENT
	TCPPeerTestClient();
	#endif
	#ifdef PROTOCOL
	protocolTest();
	#endif

	return 0;
}
