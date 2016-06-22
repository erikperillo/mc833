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
	EXIT
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

std::string sanitize(const std::string& str);
std::string desanitize(const std::string& str);
std::vector<std::string> split(const std::string& str, 
	const std::string& delim);
std::string initMessage(char message);
void addField(std::string& str, const std::string& field);

std::string hostToNetMsg(const std::string& src_user_name, 
	const std::string& dst_user_name, const std::string& content);
std::string hostToNetMsg(const Message& msg);
Message netToHostMsg(const std::string& str);
std::string hostToNetJoinGroup(const std::string& group_name);
std::string netToHostJoinGroup(const std::string& str);
std::string hostToNetExit();

#endif
