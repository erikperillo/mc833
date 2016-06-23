	#include "protocol.h"

/*ProtocolFormatError::ProtocolFormatError(const std::string& msg): msg(msg)
{;}

std::string ProtocolFormatError::getMessage() const
{
	return this->msg;
}*/

std::string lower(const std::string& str)
{
	std::string res(str);

	for(unsigned i=0; i<res.size(); i++)
		if(res[i] >= 'A' && res[i] <= 'Z')
			res[i] = res[i] - ('Z' - 'z');

      return res;
}

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

std::string join(const std::vector<std::string>& tokens, 
	const std::string& delim, int pos)
{
	std::string str;

	for(unsigned i=pos; i<tokens.size()-1; i++)
		str += tokens[i] + delim;
	if(tokens.size() - pos > 0)
		str += tokens[tokens.size()-1];

	return str;
}

std::string initMessage(char message)
{
	std::string str;

	str.push_back(message);

	return str + SEP;
}

char getMessageType(const std::string& str)
{
	return (str.size() > 0)?str[0]:UNDEFINED;
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

std::string hostToNetRet(char code)
{
	return initMessage(code);
}

std::string hostToNetError(char err_code)
{
	return hostToNetRet(err_code);
}

std::string hostToNetRegister(const std::string& name)
{
	std::string str(SEP);
	addField(str, REGISTER_CMD);
	addField(str, name);
	return str;
}

std::string netToHostRegister(const std::string& str)
{
	std::vector<std::string> tokens;

	tokens = split(str, SEP);
	if(tokens.size() < 2)
		return "";

	return desanitize(tokens[1]);
}
