#include "chat.h"

Chat::Chat()
{
	this->addGroup(Group(CHAT_ONLINE_GROUP_NAME));
	this->addGroup(Group(CHAT_OFFLINE_GROUP_NAME));
}

void Chat::addGroup(const Group& group)
{
	this->groups.push_back(group);
}

void Chat::delGroup(const std::string& group_name)
{
	for(unsigned i=0; i<this->groups.size(); i++)
		if(this->groups[i].getName() == group_name)
		{
			this->groups.erase(this->groups.begin() + i);
			break;
		}
}

void Chat::addUser(const User& user)
{
	this->users.push_back(user);
}

void Chat::delUser(const std::string& user_name)
{
	for(unsigned i=0; i<this->users.size(); i++)
		if(this->users[i].getName() == user_name)
		{
			this->users.erase(this->users.begin() + i);
			break;
		}
}

void Chat::addMessage(const Message& msg)
{
	this->messages.push_back(msg);
}

void Chat::delMessage(const unsigned long& msg_id)
{
	for(unsigned i=0; i<this->messages.size(); i++)
		if(this->messages[i].getId() == msg_id)
		{
			this->messages.erase(this->messages.begin() + i);
			break;
		}
}
