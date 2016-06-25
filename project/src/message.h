#ifndef __MESSAGE_HEADER__
#define __MESSAGE_HEADER__

#include <string>

class Message
{
	private:
	std::string src_user_name;
	std::string dst_user_name;
	std::string content;

	public:
	Message();
	Message(const Message& msg);
	Message(const std::string src_user_name, const std::string dst_user_name,
		const std::string& content);
	std::string getContent() const;
	std::string getSrcUserName() const;
	std::string getDstUserName() const;
};

#endif
