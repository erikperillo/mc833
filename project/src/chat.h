#include <vector>
#include "group.h"
#include "message.h"
#include "user.h"

#define CHAT_ONLINE_GROUP_NAME "online"
#define CHAT_OFFLINE_GROUP_NAME "offline"

class Chat
{
	private:
	std::vector<Message> messages;
	std::vector<User> users;
	std::vector<Group> groups;

	public:
	Chat();
	void addGroup(const Group& group);
	void delGroup(const std::string& group_name);
	void addUser(const User& user);
	void delUser(const std::string& user_name);
	void addMessage(const Message& msg);
	void delMessage(const unsigned long& msg_id);
	void addUserToGroup(const std::string& user_name, 
		const std::string& group_name);
	void delUserFromGroup(const std::string& user_name, 
		const std::string& group_name);
};
