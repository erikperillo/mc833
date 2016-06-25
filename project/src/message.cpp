#include "message.h"

Message::Message()
{;}

Message::Message(const Message& msg)
{
	*this = msg;
}

Message::Message(const std::string src_user_name, 
		const std::string dst_user_name, const std::string& content):
	src_user_name(src_user_name), dst_user_name(dst_user_name), content(content)
{;}

std::string Message::getContent() const
{
	return this->content;
}

std::string Message::getSrcUserName() const
{
	return this->src_user_name;
}

std::string Message::getDstUserName() const
{
	return this->dst_user_name;
}
