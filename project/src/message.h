#ifndef __MESSAGE_HEADER__
#define __MESSAGE_HEADER__

#include <string>

class Message
{
	private:
	static unsigned long num_messages;
	std::string src_user_name;
	std::string dst_user_name;
	std::string content;
	unsigned long id;

	public:
	Message(const std::string src_user_name, const std::string dst_user_name,
		const std::string& content);
	std::string getContent() const;
	std::string getSrcUserName() const;
	std::string getDstUserName() const;
	unsigned long getId() const;
};

#endif
