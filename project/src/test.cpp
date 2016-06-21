#include <iostream>
#include <string>
#include "chat.h"
#include "user.h"
#include "group.h"
#include "message.h"
#include "chatview.h"
#include "peer.h"

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
	UDPPeer peer;	

	if(peer.setup("127.0.0.1", SERVER_PORT) < 0)
		error("setup");

	while(true)
	{
		if(peer.recv() < 0)
			error("recv");

		cout << "message received from "
			<< peer.getSenderIp() << ":" << peer.getSenderPort() << " > "
			<< peer.getReceivedMsg() << endl;

		if(peer.send(peer.getSenderIp(), peer.getSenderPort(), 
			peer.getReceivedMsg()) < 0)
			error("send");
	}
}

void TCPPeerTestServer()
{
	TCPPeer peer;	
	int ret;

	if(peer.setup("127.0.0.1", SERVER_PORT) < 0)
		error("setup");

	if(peer.listen() < 0)
		error("listen");

	if(peer.accept() < 0)
		error("accept");

	while(true)
	{
		ret = peer.recv();
		if(ret < 0)
			error("recv");
		else if(ret == 0)
		{
			cout << "no more messages." << endl;
			return;
		}

		cout << "message received from "
			<< peer.getSenderIp() << ":" << peer.getSenderPort() << " > "
			<< peer.getReceivedMsg() << endl;

		if(peer.send(peer.getReceivedMsg()) < 0)
			error("send");
	}
}

void UDPPeerTestClient()
{
	UDPPeer peer;	
	UDPPeer dst;
	std::string msg;

	if(peer.setup("127.0.0.1") < 0)
		error("setup");

	while(true)
	{
		msg = "";
		cout << ">>> ";
		cin >> msg;

		if(peer.send("127.0.0.1", SERVER_PORT, msg) < 0)
			error("send");

		if(peer.recv() < 0)
			error("recv");

		cout << "message received from "
			<< peer.getSenderIp() << ":" << peer.getSenderPort() << " > "
			<< peer.getReceivedMsg() << endl;
	}
}

void TCPPeerTestClient()
{
	TCPPeer peer;	
	TCPPeer dst;
	std::string msg;

	if(peer.setup("127.0.0.1", 7533) < 0)
		error("setup");

	if(peer.connect("127.0.0.1", SERVER_PORT) < 0)
		error("connect");

	while(true)
	{
		msg = "";
		cout << ">>> ";
		cin >> msg;

		if(peer.send(msg) < 0)
			error("send");

		if(peer.recv() < 0)
			error("recv");

		cout << "message received from "
			<< peer.getSenderIp() << ":" << peer.getSenderPort() << " > "
			<< peer.getReceivedMsg() << endl;
	}
}

#include "chatcontroller.h"
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

	return 0;
}
