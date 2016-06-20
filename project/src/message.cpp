#include "message.h"

unsigned long Message::num_messages = 0;

Message::Message(const std::string src_user_name, 
		const std::string dst_user_name, const std::string& content):
	src_user_name(src_user_name), dst_user_name(dst_user_name), content(content)
{
	this->id = Message::num_messages++;
}

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

unsigned long Message::getId() const
{
	return this->id;
}
