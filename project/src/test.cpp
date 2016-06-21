#include <iostream>
#include <string>
#include "chat.h"
#include "user.h"
#include "group.h"
#include "message.h"
#include "chatview.h"

using namespace std;

inline void addUserCheck(Chat& chat, const string& name)
{
	if(!chat.addUser(User(name, "127.0.0.1", 0)))
		cout << "user '" << name << "' could not be added to chat" << endl;
	else
		cout << "user '" << name << "' added to chat" << endl;
}

inline void addGroupCheck(Chat& chat, const string& name)
{
	if(!chat.addGroup(name))
		cout << "group '" << name << "' could not be added to chat" << endl;
	else
		cout << "group '" << name << "' added to chat" << endl;
}

inline void addUserToGroupCheck(Chat& chat, const string& user_name,
	const string& group_name)
{
	if(!chat.addUserToGroup(user_name, group_name))
		cout << "user '" << user_name << "' could not be added to group '" << 
		group_name << "'" << endl;
	else
		cout << "user '" << user_name << "' added to group '" << 
		group_name << "'" << endl;
}


inline void addMessageCheck(Chat& chat, const Message& msg)
{
	if(!chat.addMessage(msg))
		cout << "message #" << msg.getId() << 
		" could not be added to chat" << endl;
	else
		cout << "message #" << msg.getId() << " added to chat" << endl;
}

void chatTest()
{
	Chat chat;

	addUserCheck(chat, "erik");
	addUserCheck(chat, "larissa");
	addUserCheck(chat, "erik");
	addUserCheck(chat, "mario");
	addUserCheck(chat, "jose");
	addUserCheck(chat, "leandro");

	addGroupCheck(chat, "safados");
	addGroupCheck(chat, "safados");
	addGroupCheck(chat, "pilantras");
	addGroupCheck(chat, "safados");
	addGroupCheck(chat, "gostosos");
	addGroupCheck(chat, "gostosos");

	Message msg1 = Message("", "", "");
	Message msg2 = Message("", "", "");
	addMessageCheck(chat, msg1);
	addMessageCheck(chat, msg1);
	addMessageCheck(chat, msg2);
	addMessageCheck(chat, msg2);
	addMessageCheck(chat, msg1);

	addUserToGroupCheck(chat, "erik", "safados");
	addUserToGroupCheck(chat, "erik", "pilantras");
	addUserToGroupCheck(chat, "larissa", "pilantras");
	addUserToGroupCheck(chat, "erik", "pilantras");
	addUserToGroupCheck(chat, "erik", "kajdhf");
	addUserToGroupCheck(chat, "alkjdfh", "pilantras");
}

void chatViewTest()
{
	Chat chat;
	ChatView view(chat);

	addUserCheck(chat, "erik");
	addUserCheck(chat, "larissa");
	addUserCheck(chat, "mario");
	addUserCheck(chat, "jose");
	addUserCheck(chat, "leandro");

	addGroupCheck(chat, "safados");
	addGroupCheck(chat, "pilantras");
	addGroupCheck(chat, "safados");
	addGroupCheck(chat, "gostosos");

	Message msg1 = Message("", "", "");
	Message msg2 = Message("", "", "");
	addMessageCheck(chat, msg1);
	addMessageCheck(chat, msg2);

	addUserToGroupCheck(chat, "erik", "safados");
	addUserToGroupCheck(chat, "erik", "pilantras");
	addUserToGroupCheck(chat, "larissa", "pilantras");

	cout << "users:" << endl;
	view.printUsers();

	cout << "groups:" << endl;
	view.printGroups();

	view.printUsersFromGroup("pilantras");
}

int main()
{
	//chatTest();
	chatViewTest();
}
