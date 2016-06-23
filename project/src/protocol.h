#ifndef __PROTOCOL_HEADER__
#define __PROTOCOL_HEADER__

#include <string>
#include <regex>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "message.h"

#define SEP ":"

enum messages: char
{
	SEND_MSG,
	JOIN_GROUP,
	UNDEFINED,
	INVALID_COMMAND_ERR,
	OK,
	REGISTER,
	EXIT
};

#define REGISTER_CMD "register"
#define EXIT_CMD "exit"
#define JOIN_GROUP_CMD "joing"
#define SEND_MSG_CMD "sendmsg"
#define HELP_CMD "help"
#define HELP "help"
#define OK "ok"
#define INVALID_COMMAND_ERR "invcom"

/*
class ProtocolFormatError
{
	private:
	std::string msg;

	public:
	ProtocolFormatError(const std::string& msg);
	std::string getMessage() const;
};*/

std::string lower(const std::string& str);
std::string sanitize(const std::string& str);
std::string desanitize(const std::string& str);
std::vector<std::string> split(const std::string& str, 
	const std::string& delim);
std::string join(const std::vector<std::string>& tokens, 
	const std::string& delim=" ", int pos=0);
std::string initMessage(char message);
char getMessageType(const std::string& str);
void addField(std::string& str, const std::string& field);

std::string hostToNetMsg(const std::string& src_user_name, 
	const std::string& dst_user_name, const std::string& content);
std::string hostToNetMsg(const Message& msg);
Message netToHostMsg(const std::string& str);
std::string hostToNetJoinGroup(const std::string& group_name);
std::string netToHostJoinGroup(const std::string& str);
std::string hostToNetExit();
std::string hostToNetError(char err_code);
std::string hostToNetRet(char code);
std::string hostToNetRegister(const std::string& name);
std::string netToHostRegister(const std::string& msg);

#endif
