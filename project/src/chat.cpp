#include "chat.h"

Chat::Chat()
{;}

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

Group Chat::getGroup(const std::string& group_name) throw(ElementNotFound)
{
	for(auto it = this->groups.begin(); it != this->groups.end(); it++)
		if(it->getName() == group_name)
			return *it;

	throw ElementNotFound("group " + group_name + " could not be found");
}

std::vector<std::string> Chat::getGroupsNames()
{
	std::vector<std::string> names;

	for(auto it = this->groups.begin(); it != this->groups.end(); it++)
		names.push_back(it->getName());

	return names;
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

User Chat::getUser(const std::string& user_name) throw(ElementNotFound)
{
	for(auto it = this->users.begin(); it != this->users.end(); it++)
		if(it->getName() == user_name)
			return *it;

	throw ElementNotFound("user " + user_name + " could not be found");
}

std::vector<std::string> Chat::getUsersNames()
{
	std::vector<std::string> names;

	for(auto it = this->users.begin(); it != this->users.end(); it++)
		names.push_back(it->getName());

	return names;
}

void Chat::addUserToGroup(const std::string& user_name,
		const std::string& group_name)
{
	for(auto it = this->groups.begin(); it != this->groups.end(); it++)
		if(it->getName() == group_name)
			it->addUser(user_name);
}

void Chat::delUserFromGroup(const std::string& user_name,
		const std::string& group_name)
{
	for(auto it = this->groups.begin(); it != this->groups.end(); it++)
		if(it->getName() == group_name)
			it->delUser(user_name);
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

Message Chat::getMessage(const unsigned long& msg_id) throw(ElementNotFound)
{
	for(auto it = this->messages.begin(); it != this->messages.end(); it++)
		if(it->getId() == msg_id)
			return *it;

	throw ElementNotFound("message #" + std::to_string(msg_id) + \
		" could not be found");
}

std::vector<unsigned long> Chat::getMessagesIds()
{
	std::vector<unsigned long> ids;

	for(auto it = this->messages.begin(); it != this->messages.end(); it++)
		ids.push_back(it->getId());

	return ids;
}

