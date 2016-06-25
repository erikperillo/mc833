#include <iostream>
#include <algorithm>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>
#include "net.h"
#include "protocol.h"

using namespace std;

#define SERVER_NAME "127.0.0.1"
#define SERVER_PORT 7532
#define HASH_CUT 6

#define REGISTER_CMD "register" 
#define EXIT_CMD "exit"
#define JOIN_GROUP_CMD "joing"
#define SEND_MSG_CMD "sendmsg"
#define HELP_CMD "help"
#define INVALID_CMD_ERR "invcom"
#define NO_SRC_USER_ERR "nosrcusr"
#define NO_DST_USER_ERR "nodstusr"

vector<size_t> messages_ids;
std::mutex messages_ids_mtx;

void error(const string& msg, int ret=1)
{
	perror(msg.c_str());
	exit(ret);
}

void info(const string& msg, const string& prompt="[chat] ")
{
	cout << prompt << msg << endl;
}

void displayHelpMessage(const string& prompt="\t")
{
	info("commands:");
	info(string(HELP_CMD) + " -> display this help message", prompt);
	info(string(SEND_MSG_CMD) + " <user> <msg> -> messages <msg> to <user>", 
		prompt);
	info(string(JOIN_GROUP_CMD) + " <group_name> -> joins <group_name>", 
		prompt);
	info(string(EXIT_CMD) + " -> exits chat", prompt);
}

string lower(const string& str)
{
	string res(str);

	for(unsigned i=0; i<res.size(); i++)
		if(res[i] >= 'A' && res[i] <= 'Z')
			res[i] = res[i] - ('Z' - 'z');

    return res;
}

string cmdToNetMsg(const string& cmd)
{
	vector<string> tokens;
	string msg(FIELD_SEP);

	tokens = split(cmd, " ");
	for(auto const& tok: tokens)
		msg += sanitize(tok) + FIELD_SEP;

	return msg;
}

int handle(const string& answer, const string& prompt="[server] ")
{
	char header;
	int ret = 0;

	header = netToHostHeader(answer);

	switch(header)
	{
		case OK:
			info("OK", prompt);
			break;
		case INVALID_REQUEST_ERR:
			info("ERROR: invalid request", prompt);
			break;
		case MSG_QUEUED:
		{
			size_t msg_id;
			msg_id = netToHostMsgQueued(answer);
			info("message #" + to_string(msg_id).substr(0, HASH_CUT) 
				+ " queued to be sent!", prompt);
			messages_ids_mtx.lock();
			messages_ids.push_back(msg_id);
			messages_ids_mtx.unlock();
			break;
		}
		case MSG_SENT:
		{
			size_t msg_id;
			msg_id = netToHostMsgSent(answer);
			info("message #" + to_string(msg_id).substr(0, HASH_CUT) 
				+ " was sent to destination!", prompt);
			messages_ids_mtx.lock();
			messages_ids.erase(remove(messages_ids.begin(), messages_ids.end(),
				msg_id), messages_ids.end());
			messages_ids_mtx.unlock();
			break;
		}
		case MSG_INCOMING:
		{
			Message msg = netToHostMsgIncoming(answer);
			info(msg.getContent(), "[msg from " + msg.getSrcUserName() + "] ");
			break;
		}
		case MSG_EXISTS:
			info("message already queued to be sent", prompt);
			break;
		case NO_MSG_DST_ERR:
			info("ERROR: destiny user does not exist", prompt);
			ret = -1;
			break;
		case USER_EXISTS_ERR:
			info("ERROR: cannot register, username already exists", prompt);
			ret = -1;
			break;
		default:
			info("unknown answer", prompt);
			break;
	}

	return ret;
}

void observe(int sock, string prompt)
{
	NetAddr src;
	NetMessage msg;
	NetReceiver receiver;

	receiver = NetReceiver(sock);

	while(true)
	{
		//receiving server response
		msg = receiver.recv();
		if(msg.getErrCode() < 0)
			error("recv");	
		if(msg.getErrCode() == 0)
			break;

		//displaying server response
		src = msg.getSrcAddr();
		cout << "[" << src.getIp() << ":" << src.getPort() << "]"
			<< " " << msg.getContent() << endl;

		//handling answer
		//for(auto const& str: split(msg.getContent(), string(1, NET_SEP)))
		//	handle(sock, str);
		handle(msg.getContent());

		cout << prompt << flush;
	}
}

int registerUser(int sock, const string& name)
{
	int ret;
	NetMessage msg;
	NetReceiver receiver;

	receiver = NetReceiver(sock);

	//sending registering message to server
	info("registering user '" + name + "'...");
	ret = send(sock, hostToNetRegister(name));
	if(ret <= 0)
		error("send");	

	//receiving answer
	msg = receiver.recv();
	if(msg.getErrCode() < 0)
		error("recv");	

	return handle(msg.getContent());
}

string cmdToNetMsg(const string& cmd, const string& user_name)
{
	vector<string> tokens;
	string arg_1;
	string arg_2;
	string arg_3;
	string str;

	tokens = split(cmd, " ");

	switch(tokens.size())
	{
		case 0:
			break;

		case 1:
			arg_1 = lower(tokens[0]);
			if(arg_1 == EXIT_CMD)
				str = hostToNetMsg(EXIT);
			break;

		case 2:		
			arg_1 = lower(tokens[0]);
			arg_2 = tokens[1];
			if(arg_1 == JOIN_GROUP_CMD)
				str = hostToNetJoinGroup(arg_2);
			break;

		//number of args >= 3
		default:
			arg_1 = lower(tokens[0]);
			arg_2 = tokens[1];
			if(arg_1 == SEND_MSG_CMD)
			{
				arg_3 = join(tokens, " ", 2);
				str = hostToNetSendMsg(user_name, arg_2, arg_3);
			}
			break;
	}

	return str;
}

void client(string& ip, unsigned short port, string& name)
{
	NetAddr server(ip, port);	
	int sock;
	int ret;
	NetMessage msg;
	NetAddr src;
	string cmd;
	string request;
	string prompt;
	thread thr;
	
	prompt = "$[" + name + "] ";

	if(server.getErrCode() < 0)
		error("NetAddr");

	sock = getSocket(SOCK_STREAM);	
	if(sock < 0)
		error("getSocket");

	if(connect(sock, server) < 0)
		error("connect");	
		
	info("connected to " + 
		server.getIp() + ":" + to_string(server.getPort()));

	if(registerUser(sock, name) < 0)
		return;
	
	thr = thread(observe, sock, prompt);

	cout << prompt << flush;
	while(true)
	{
		//getting command from console
		getline(cin, cmd);

		if(lower(cmd) == HELP_CMD)
		{
			displayHelpMessage();
			cout << prompt << flush;
			continue;
		}

		request = cmdToNetMsg(cmd, name);
		if(request.empty())
		{
			info("invalid command. use 'help' for more info.", prompt);
			cout << prompt << flush;
			continue;
		}

		//sending message
		ret = send(sock, request);
		if(ret < 0)
			error("error sending message to server");	
		if(ret == 0)
			break;
	}

	if(thr.joinable())
		thr.join();

	cout << "connection ended." << endl;
}

int main(int argc, char** argv)
{
	if(argc < 4)
	{
		info("usage: client <ip> <port> <name>");
		return 0;
	}

	string ip(argv[1]), port(argv[2]), name(argv[3]);
	client(ip, (unsigned short)stoi(port), name);

	return 0;
}
