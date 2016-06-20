#include "group.h"

Group::Group(const std::string& name): name(name)
{;}

void Group::addUser(const std::string& user_name)
{
	if(this->has(user_name))
		return;

	this->users_names.push_back(user_name);
}

void Group::delUser(const std::string& user_name)
{
	for(unsigned i=0; i<this->users_names.size(); i++)
		if(this->users_names[i] == user_name)
		{
			this->users_names.erase(this->users_names.begin() + i);
			break;
		}
}

bool Group::has(const std::string& user_name) const
{
	for(unsigned i=0; i<this->users_names.size(); i++)
		if(this->users_names[i] == user_name)
			return true;

	return false;
}

std::string Group::getName() const
{
	return this->name;
}
