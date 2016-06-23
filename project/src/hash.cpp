#include "hash.h"

namespace std
{
	std::size_t hash<Message>::operator()(const Message& msg) const
	{
		std::size_t const h1 (std::hash<std::string>()(msg.getSrcUserName()));
		std::size_t const h2 (std::hash<std::string>()(msg.getDstUserName()));
		std::size_t const h3 (std::hash<std::string>()(msg.getContent()));

		return h1 ^ ((h2 ^ (h3 << 1)) << 1);
	}
}
