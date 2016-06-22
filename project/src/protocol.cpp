#include "protocol.h"

/*ProtocolFormatError::ProtocolFormatError(const std::string& msg): msg(msg)
{;}

std::string ProtocolFormatError::getMessage() const
{
	return this->msg;
}*/

std::string sanitize(const std::string& str)
{
	return std::regex_replace(str, std::regex(SEP), "\\sep");
}

std::string desanitize(const std::string& str)
{
	return std::regex_replace(str, std::regex("\\\\sep"), SEP);
}

void addField(std::string& str, const std::string& field)
{
	str += sanitize(field) + SEP;
}

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> tokens;
	std::string token;
	unsigned i=0;

	while(i < str.size())
	{
		while(i < str.size() && str.compare(i, delim.size(), delim))
			token.push_back(str[i++]);	

		if(!token.empty())
			tokens.push_back(token);
		token.clear();
		i += delim.size();
	}

	return tokens;
}

std::string initMessage(char message)
{
	std::string str;

	str.push_back(message);

	return str + SEP;
}

std::string hostToNetMsg(const std::string& src_user_name, 
	const std::string& dst_user_name, const std::string& content)
{
	std::string str = initMessage(SEND_MSG);

	addField(str, src_user_name);
	addField(str, dst_user_name);
	addField(str, content);

	return str;
}

std::string hostToNetMsg(const Message& msg)
{
	std::string str = initMessage(SEND_MSG);

	addField(str, msg.getSrcUserName());
	addField(str, msg.getDstUserName());
	addField(str, msg.getContent());

	return str;
}

Message netToHostMsg(const std::string& str)
{
	std::vector<std::string> tokens;

	tokens = split(str, SEP);
	if(tokens.size() < 4)
		return Message("", "", "");	

	return Message(desanitize(tokens[1]), desanitize(tokens[2]), 
		desanitize(tokens[3]));
}

std::string hostToNetJoinGroup(const std::string& group_name)
{
	std::string str = initMessage(JOIN_GROUP);

	addField(str, group_name);

	return str;
}

std::string netToHostJoinGroup(const std::string& str)
{
	std::vector<std::string> tokens;

	tokens = split(str, SEP);
	if(tokens.size() < 2)
		return "";

	return desanitize(tokens[1]);
}

std::string hostToNetExit()
{
	return initMessage(EXIT);
}
