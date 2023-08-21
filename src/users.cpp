#include "../inc/Users.hpp"

Users::Users()
{
    // Constructor implementation
}

Users::~Users()
{
    // Destructor implementation
}

void Users::createUser(int fd, const std::string& username, const std::string& nick, const std::string& hostname, const std::string& realName)
{
    addAllUserFd(fd);
    setHostName(fd, hostname);
    setUserName(fd, username);
    setNickName(fd, nick);
    setRealName(fd, realName);
    setUserAuth(fd, false);
    struct UsersAuth userAuth(false, false, false);
    setUserAuthStruct(fd, userAuth);
    std::cout << "user created" << "\n";
}

std::string Users::getUsername(int id) const
{
    std::map<int, std::string>::const_iterator it = username.find(id);
    if (it != username.end())
    {
        return it->second;
    }
    return "";
}

void Users::setUserName(int id, const std::string& name)
{
    username[id] = name;
}


std::string Users::getNickname(int id) const
{
    std::map<int, std::string>::const_iterator it = nickname.find(id);
    if (it != nickname.end())
    {
        return it->second;
    }
    return "";
}

int Users::getNickname(const std::string& nick) const
{
	for (std::map<int, std::string>::const_iterator it = nickname.begin(); it != nickname.end(); ++it)
	{
		if (it->second == nick)
		{
			return it->first;
		}
	}
	return -1;
}

void Users::setNickName(int id, const std::string& name)
{
    nickname[id] = name;
}

bool Users::controlNickName(const std::string name)
{
	for (size_t i = 0; i < this->nickname.size(); i++)
	{
		if (name == trim(this->nickname[i]))
			return false;
	}
	return true;
}

std::string Users::getHostname(int id) const
{
    std::map<int, std::string>::const_iterator it = hostname.find(id);
    if (it != hostname.end())
    {
        return it->second;
    }
    return "";
}

void Users::setHostName(int id, const std::string& host)
{
    hostname[id] = host;
}

std::string Users::getRealname(int id) const
{
    std::map<int, std::string>::const_iterator it = realname.find(id);
    if (it != realname.end())
    {
        return it->second;
    }
    return "";
}

void Users::setRealName(int id, const std::string& name)
{
    realname[id] = name;
}

std::vector<std::string> Users::getChannels() const
{
    return channels;
}

void Users::addChannel(const std::string& channel)
{
    channels.push_back(channel);
}

void Users::removeChannel(const std::string& channel)
{
    std::vector<std::string>::iterator it = std::find(channels.begin(), channels.end(), channel);
    if (it != channels.end())
    {
        channels.erase(it);
    }
}

int Users::getAllUsersFd(int index) const
{
    if (index >= 0 && index < (int)allUsersFd.size())
        return allUsersFd[index];
    return -1;
}

std::vector<int> Users::getAllUsersFd() const
{
    return(this->allUsersFd);
}

void Users::addAllUserFd(int fd)
{
    allUsersFd.push_back(fd);
}

bool Users::getUserAuth(int fd) const
{
    std::map<int, bool>::const_iterator it = usersAuth.find(fd);
    if (it != usersAuth.end())
    {
        return it->second;
    }
    return false;
}

void Users::setUserAuth(int fd, bool auth)
{
    usersAuth[fd] = auth;
}

void Users::autoUpdateUserAuth(int fd)
{
    std::map<int, bool>::iterator it = usersAuth.find(fd);
    if (it != usersAuth.end())
    {
        bool auth_pass = getUserAuthStruct(fd).userPass;
        bool auth_nick = getUserAuthStruct(fd).userNick;
        bool auth_user = getUserAuthStruct(fd).userUser;
        bool auth = auth_pass && auth_nick && auth_user;
        setUserAuth(fd, auth);
    }
}

void Users::setUserAuthStruct(int fd, struct UsersAuth auth)
{
    usersAuthStruct[fd] = auth;
}

struct UsersAuth Users::getUserAuthStruct(int fd) const
{
    std::map<int, UsersAuth>::const_iterator it = usersAuthStruct.find(fd);
    if (it != usersAuthStruct.end())
    {
        return it->second;
    }
    return UsersAuth();
}

void Users::setUserAuthStruct(int fd, const std::string& header, bool auth)
{
    std::map<int, UsersAuth>::iterator it = usersAuthStruct.find(fd);
    if (it != usersAuthStruct.end())
    {
        if (header == "pass")
            it->second.userPass = auth;
        else if (header == "nick")
            it->second.userNick = auth;
        else if (header == "user")
            it->second.userUser = auth;
    }
}

bool Users::getUserAuthStruct(int fd, const std::string& header) const
{
    std::map<int, UsersAuth>::const_iterator it = usersAuthStruct.find(fd);
    if (it != usersAuthStruct.end())
    {
        if (header == "pass")
            return it->second.userPass;
        else if (header == "nick")
            return it->second.userNick;
        else if (header == "user")
            return it->second.userUser;
    }
    return false;
}

void Users::setUserWelcomeMessage(int fd, bool welcome)
{
    usersWelcomeMessage[fd] = welcome;
}

bool Users::getUserWelcomeMessage(int fd) const
{
    std::map<int, bool>::const_iterator it = usersWelcomeMessage.find(fd);
    if (it != usersWelcomeMessage.end())
    {
        return it->second;
    }
    return false;
}

void Users::setUserMod(int id, int mod)
{
    usersMod[id] = mod;
}

int Users::getUserMod(int id) const
{
    std::map<int, int>::const_iterator it = usersMod.find(id);
    if (it != usersMod.end())
    {
        return it->second;
    }
    return -1;
}

std::string Users::getPrefix(const int &fd) const
{
    return ":" + getNickname(fd) + "!" + getUsername(fd) + "@" + getHostname(fd);
}


void Users::removeUserAllInfo(int fd)
{
	std::map<int, std::string>::iterator it = nickname.find(fd);
	if (it != nickname.end())
	{
		nickname.erase(it);
	}
	it = hostname.find(fd);
	if (it != hostname.end())
	{
		hostname.erase(it);
	}
	it = realname.find(fd);
	if (it != realname.end())
	{
		realname.erase(it);
	}
	std::map<int, bool>::iterator it2 = usersAuth.find(fd);
	if (it2 != usersAuth.end())
	{
		usersAuth.erase(it2);
	}
	std::map<int, UsersAuth>::iterator it3 = usersAuthStruct.find(fd);
	if (it3 != usersAuthStruct.end())
	{
		usersAuthStruct.erase(it3);
	}
	std::map<int, bool>::iterator it4 = usersWelcomeMessage.find(fd);
	if (it4 != usersWelcomeMessage.end())
	{
		usersWelcomeMessage.erase(it4);
	}
	std::map<int, int>::iterator it5 = usersMod.find(fd);
	if (it5 != usersMod.end())
	{
		usersMod.erase(it5);
	}
	std::vector<int>::iterator it6 = std::find(allUsersFd.begin(), allUsersFd.end(), fd);
	if (it6 != allUsersFd.end())
	{
		allUsersFd.erase(it6);
	}

	std::map<int, std::string>::iterator it7 = this->username.find(fd);
	if (it7 != this->username.end())
	{
		this->username.erase(it7);
	}
}