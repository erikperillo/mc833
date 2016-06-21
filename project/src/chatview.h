#ifndef __CHATVIEW_HEADER__
#define __CHATVIEW_HEADER__

#include "chat.h"
#include <iostream>
#include <string>

class ChatView
{
	private:
	unsigned col_w;
	Chat& model;
	std::ostream& out;
	//methods
	void setColWidth(const std::vector<std::string>& words);
	std::string center(const std::string& word);
	std::string col(const std::string& word);
	std::string sep(unsigned cols=1);
	void printTable(const std::vector<std::string>& rows,
		const std::string header=std::string());
	void printTable(const std::vector<std::vector<std::string>>& cols, 
		const std::vector<std::string>& header=std::vector<std::string>());

	public:
	ChatView(Chat& model, std::ostream& out=std::cout);
	void printUsers(const std::string& title="users");
	void printGroups(const std::string& title="groups");
	bool printUsersFromGroup(const std::string& group_name);
};

#endif
