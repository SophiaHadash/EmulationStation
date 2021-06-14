#include "UserConfig.h"

#include <pugixml/src/pugixml.hpp>
#include "utils/FileSystemUtil.h"
#include <iostream>
#include "Log.h"
#include <cassert>

UserConfig* UserConfig::sInstance = NULL;

void UserConfig::init()
{
    assert(!sInstance);
    sInstance = new UserConfig();
}

UserConfig* UserConfig::get()
{
    assert(sInstance);
    return sInstance;
}

std::string UserConfig::getConfigPath()
{
    std::string path = Utils::FileSystem::getHomePath();
    path += "/.emulationstation/es_users.cfg";
    return path;
}

std::string UserConfig::toLower(std::string str)
{
    for(unsigned int i = 0; i < str.length(); i++)
    {
        str[i] = (char)tolower(str[i]);
    }

    return str;
}

UserConfig::UserConfig()
{
    UserConfig::loadFromXML();
}

User UserConfig::getActiveUser(){
    return mUserMap[mCurrentUser];
}

void UserConfig::setActiveUser(const std::string userName){
    mCurrentUser = userName;
}

std::map<std::string, User>* UserConfig::getUsers(){
    return &mUserMap;
}

User* UserConfig::stringsToUser(std::string userName, std::string fullName, std::string image)
{
    User *u = new User();
    u->name = userName;
    u->fullName = fullName;
    u->image = image;
    return u;
}

bool UserConfig::loadFromXML()
{
    std::string path = getConfigPath();
    if(!Utils::FileSystem::exists(path))
        return false;

    pugi::xml_document doc;
    pugi::xml_parse_result res = doc.load_file(path.c_str());

    if(!res)
    {
        LOG(LogError) << "Error parsing user config: " << res.description();
        return false;
    }

    pugi::xml_node root = doc.child("userData");
    if(!root)
        return false;

    mCurrentUser = root.child("currentUser").value();

    pugi::xml_node userList = root.child("userList");

	for(pugi::xml_node user = userList.child("user"); user; user = user.next_sibling("user"))
	{
		std::string userName = user.child("name").text().as_string();
        std::string fullName = user.child("fullName").text().as_string();
        std::string image = user.child("image").text().as_string();

        User* u = stringsToUser(userName, fullName, image);
		mUserMap[toLower(userName)] = *u;
	}

	return true;
}

void UserConfig::writeToXML()
{
    std::string path = getConfigPath();
    pugi::xml_document doc;

    pugi::xml_node root = doc.child("userData");
    if(!root)
        root = doc.append_child("userData");

    pugi::xml_node currentUser = root.append_child("currentUser");
    pugi::xml_node currentUserInner = currentUser.append_child(pugi::node_pcdata);
    currentUserInner.set_name("currentUser");
    currentUserInner.set_value(mCurrentUser.c_str());
    pugi::xml_node userList = root.append_child("userList");

	typedef std::map<std::string, User>::const_iterator it_type;
	for(it_type iterator = mUserMap.cbegin(); iterator != mUserMap.cend(); iterator++)
	{

	    pugi::xml_node user = userList.append_child("user");

        pugi::xml_node name = user.append_child("name");
	    pugi::xml_node nameInner = name.append_child(pugi::node_pcdata);
        nameInner.set_name("name");
        nameInner.set_value(iterator->second.name.c_str());

        pugi::xml_node fullName = user.append_child("fullName");
        pugi::xml_node fullNameInner = fullName.append_child(pugi::node_pcdata);
        fullNameInner.set_name("fullName");
        fullNameInner.set_value(iterator->second.fullName.c_str());

        pugi::xml_node image = user.append_child("image");
        pugi::xml_node imageInner = image.append_child(pugi::node_pcdata);
        imageInner.set_name("image");
        imageInner.set_value(iterator->second.image.c_str());

	}

    doc.save_file(path.c_str(), PUGIXML_TEXT("  "));
}
