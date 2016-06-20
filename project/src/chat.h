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
	void addGroup(const Group& group);
	void delGroup(const std::string& group_name);
	Group getGroup(const std::string& group_name) throw(ElementNotFound);
	std::vector<std::string> getGroupsNames();
	//users methods
	void addUser(const User& user);
	void delUser(const std::string& user_name);
	User getUser(const std::string& user_name) throw(ElementNotFound);
	std::vector<std::string> getUsersNames();
	void addUserToGroup(const std::string& user_name, 
		const std::string& group_name);
	void delUserFromGroup(const std::string& user_name, 
		const std::string& group_name);
	//messages methods
	void addMessage(const Message& msg);
	void delMessage(const unsigned long& msg_id);
	Message getMessage(const unsigned long& msg_id) throw(ElementNotFound);
	std::vector<unsigned long> getMessagesIds();
};
