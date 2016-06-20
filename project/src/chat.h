#ifndef __CHAT_HEADER__
#define __CHAT_HEADER__

#include <vector>
#include "group.h"
#include "message.h"
#include "user.h"
#include "elementnotfound.h"

class Chat
{
	private:
	std::vector<Message> messages;
	std::vector<User> users;
	std::vector<Group> groups;

	public:
	Chat();
	//groups methods
	bool addGroup(const std::string& group_name);
	bool delGroup(const std::string& group_name);
	bool hasGroup(const std::string& group_name);
	Group getGroup(const std::string& group_name) const throw(ElementNotFound);
	std::vector<std::string> getGroupsNames() const;
	//users methods
	bool addUser(const User& user);
	bool delUser(const std::string& user_name);
	bool hasUser(const std::string& user_name);
	User getUser(const std::string& user_name) const throw(ElementNotFound);
	std::vector<std::string> getUsersNames() const;
	bool addUserToGroup(const std::string& user_name, 
		const std::string& group_name);
	bool delUserFromGroup(const std::string& user_name, 
		const std::string& group_name);
	//messages methods
	bool addMessage(const Message& msg);
	bool delMessage(const unsigned long& msg_id);
	bool hasMessage(const unsigned long& msg_id);
	Message getMessage(const unsigned long& msg_id) const throw(ElementNotFound);
	std::vector<unsigned long> getMessagesIds() const;
};

#endif
