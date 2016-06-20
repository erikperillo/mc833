#include "chat.h"

Chat::Chat()
{;}

bool Chat::addGroup(const std::string& group_name)
{
	//checking to see if group is already inserted
	for(auto const& g: this->groups)
		if(g.getName() == group_name)
			return false;

	this->groups.push_back(Group(group_name));
	return true;
}

bool Chat::delGroup(const std::string& group_name)
{
	for(unsigned i=0; i<this->groups.size(); i++)
		if(this->groups[i].getName() == group_name)
		{
			this->groups.erase(this->groups.begin() + i);
			return true;
		}

	return false;
}

bool Chat::hasGroup(const std::string& group_name)
{
	for(auto const& g: this->groups)
		if(g.getName() == group_name)
			return true;

	return false;
}

Group Chat::getGroup(const std::string& group_name) const throw(ElementNotFound)
{
	for(auto const& g: this->groups)
		if(g.getName() == group_name)
			return g;

	throw ElementNotFound("group " + group_name + " could not be found");
}

std::vector<std::string> Chat::getGroupsNames() const
{
	std::vector<std::string> names;

	for(auto const& g: this->groups)
		names.push_back(g.getName());

	return names;
}

bool Chat::addUser(const User& user)
{
	//checking to see if group is already inserted
	for(auto const& g: this->users)
		if(g.getName() == user.getName())
			return false;

	this->users.push_back(user);
	return true;
}

bool Chat::delUser(const std::string& user_name)
{
	for(unsigned i=0; i<this->users.size(); i++)
		if(this->users[i].getName() == user_name)
		{
			this->users.erase(this->users.begin() + i);
			return true;
		}

	return false;
}

bool Chat::hasUser(const std::string& user_name)
{
	for(auto const& u: this->users)
		if(u.getName() == user_name)
			return true;

	return false;
}

User Chat::getUser(const std::string& user_name) const throw(ElementNotFound)
{
	for(auto const& u: this->users)
		if(u.getName() == user_name)
			return u;

	throw ElementNotFound("user " + user_name + " could not be found");
}

std::vector<std::string> Chat::getUsersNames() const
{
	std::vector<std::string> names;

	for(auto const& u: this->users)
		names.push_back(u.getName());

	return names;
}

bool Chat::addUserToGroup(const std::string& user_name,
		const std::string& group_name)
{
	if(!this->hasUser(user_name))
		return false;

	for(auto& g: this->groups)
		if(g.getName() == group_name)
			return g.addUser(user_name);

	return false;
}

bool Chat::delUserFromGroup(const std::string& user_name,
		const std::string& group_name)
{
	for(auto& g: this->groups)
		if(g.getName() == group_name)
			return g.delUser(user_name);

	return false;
}

bool Chat::addMessage(const Message& msg)
{
	for(auto const& m: this->messages)
		if(m.getId() == msg.getId())
			return false;

	this->messages.push_back(msg);
	return true;
}

bool Chat::delMessage(const unsigned long& msg_id)
{
	for(unsigned i=0; i<this->messages.size(); i++)
		if(this->messages[i].getId() == msg_id)
		{
			this->messages.erase(this->messages.begin() + i);
			return true;
		}

	return false;
}

bool Chat::hasMessage(const unsigned long& msg_id)
{
	for(auto const& m: this->messages)
		if(m.getId() == msg_id)
			return true;

	return false;
}

Message Chat::getMessage(const unsigned long& msg_id) const throw(ElementNotFound)
{
	for(auto const& m: this->messages)
		if(m.getId() == msg_id)
			return m;

	throw ElementNotFound("message #" + std::to_string(msg_id) + \
		" could not be found");
}

std::vector<unsigned long> Chat::getMessagesIds() const
{
	std::vector<unsigned long> ids;

	for(auto it = this->messages.begin(); it != this->messages.end(); it++)
		ids.push_back(it->getId());

	return ids;
}
