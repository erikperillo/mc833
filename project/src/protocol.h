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
#include "net.h"

#define FIELD_SEP ";"
#define MSG_SEP (std::string(1, NET_SEP))

enum messages: char
{
	SEND_MSG,
	JOIN_GROUP,
	UNDEFINED,
	OK,
	HELP,
	REGISTER,
	EXIT,
	MSG_QUEUED,
	INVALID_REQUEST_ERR,
	MSG_EXISTS_ERR,
	USER_EXISTS_ERR
};

/*
class ProtocolFormatError
{
	private:
	std::string msg;

	public:
	ProtocolFormatError(const std::string& msg);
	std::string getMessage() const;
};*/

//std::string lower(const std::string& str);
//low-level methods
std::string sanitize(const std::string& str);
std::string desanitize(const std::string& str);
std::vector<std::string> split(const std::string& str, 
	const std::string& delim);

std::string initMsg(char message);
void addField(std::string& str, const std::string& field);
void endMsg(const std::string& str);
std::string join(const std::vector<std::string>& tokens, 
	const std::string& delim=" ", int pos=0);

//general methods
std::string hostToNetHeader(char message);
char netToHostHeader(const std::string& str);
std::string hostToNetMsg(char message);
std::string hostToNetMsg(char message, std::vector<std::string> args);
std::vector<std::string> netToHostMsg(std::string msg);

std::string hostToNetRegister(const std::string& name);
std::string netToHostRegister(const std::string& msg);

std::string hostToNetSendMsg(const std::string& msg);
std::string hostToNetSendMsg(const std::string& src_user_name, 
	const std::string& dst_user_name, const std::string& content);
std::string hostToNetSendMsg(const Message& msg);
Message netToHostSendMsg(const std::string& str);

std::string hostToNetJoinGroup(const std::string& group_name);
std::string netToHostJoinGroup(const std::string& str);

std::string hostToNetExit();
std::string hostToNetHelp();

#endif
