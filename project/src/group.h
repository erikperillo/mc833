#include <string>
#include <vector>

class Group
{
	private:
	std::string name;
	std::vector<std::string> users_names;

	public:
	Group(const std::string& name);
	void addUser(const std::string& user_name);
	void delUser(const std::string& user_name);
	bool has(const std::string& user_name) const;
	std::string getName() const;
};
