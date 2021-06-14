#pragma once
#ifndef ES_CORE_USER_CONFIG_H
#define ES_CORE_USER_CONFIG_H

#include <map>
#include <sstream>
#include <vector>

namespace pugi { class xml_node; }


struct User
{
public:
    std::string name;
    std::string fullName;
    std::string image;

    User()
	{

	}
};

class UserConfig
{
public:
    static void init();
    static UserConfig* get();

    UserConfig();


    User getActiveUser();
    void setActiveUser(std::string userName);

    std::map<std::string, User> *getUsers();

	bool loadFromXML();
	void writeToXML();

    static std::string getConfigPath();

private:
    static UserConfig* sInstance;

    std::string toLower(std::string str);

	std::map<std::string, User> mUserMap;
	std::string mCurrentUser;

    User* stringsToUser(std::string userName, std::string fullName, std::string image);
};

#endif // ES_CORE_USER_CONFIG_H
