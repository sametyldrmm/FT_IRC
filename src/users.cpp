#include "../inc/Users.hpp"

Users::Users()
{
    userCount = 0;
    // Constructor implementation

}

Users::~Users()
{
    // Destructor implementation
}

void Users::createUser(int fd,const std::string& username,const std::string& nick ,const std::string& hostname,const std::string& realName)
{
    setAllUserFd(fd);
    setHostName(fd,hostname);
    setUserName(fd,username);
    setNickName(fd,nick);
    setRealName(fd,realName);
    setUserFd(getUsername(fd),fd);
    setUserAuth(getUsername(fd),false);
    std::vector<bool> temp;
    temp.push_back(false);
    temp.push_back(false);
    temp.push_back(false);
    setUserAuthVec(getUsername(fd),temp);
    userCount++;
    std::cout << "user create" << "\n";
}

// Getters

int Users::getAllUsersFd(int index) const
{
    return (this->allUsersFd[index]);
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

std::string Users::getNickname(int id) const
{
    std::map<int, std::string>::const_iterator it = nickname.find(id);
    if (it != nickname.end())
    {
        return it->second;
    }
    return "";
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

std::string Users::getRealname(int id) const
{
    std::map<int, std::string>::const_iterator it = realname.find(id);
    if (it != realname.end())
    {
        return it->second;
    }
    return "";
}

std::vector<std::string> Users::getChannels() const
{
    return channels;
}

int Users::getUserFd(const std::string &username) const
{
    std::map<std::string, int>::const_iterator it = usersFd.find(username);
    if (it != usersFd.end())
    {
        return it->second;
    }
    return -1; // Return -1 if not found
}

bool Users::getUserAuth(const std::string &username) const
{
    std::map<std::string, bool>::const_iterator it = usersAuth.find(username);
    if (it != usersAuth.end())
    {
        return it->second;
    }
    return false;
}

std::vector<bool> Users::getUserAuthVec(const std::string &username) const
{
    std::map<std::string, std::vector<bool> >::const_iterator it = usersAuthVec.find(username);
    if (it != usersAuthVec.end())
    {
        return it->second;
    }
    return std::vector<bool>(); // Return an empty vector if not found
}

// Setters
void Users::setAllUserFd(int value)
{
    this->allUsersFd.push_back(value);
}

void Users::setUserName(int id, const std::string &name)
{
    username[id] = name;
}

void Users::setNickName(int id, const std::string &name)
{
    nickname[id] = name;
}

void Users::setHostName(int id, const std::string &host)
{
    hostname[id] = host;
}

void Users::setRealName(int id, const std::string &name)
{
    realname[id] = name;
}

void Users::addChannel(const std::string &channel)
{
    channels.push_back(channel);
}

void Users::removeChannel(const std::string &channel)
{
    std::vector<std::string>::iterator it = std::find(channels.begin(), channels.end(), channel);
    if (it != channels.end())
    {
        channels.erase(it);
    }
}

void Users::setUserFd(const std::string &username, int fd)
{
    usersFd[username] = fd;
}

void Users::setUserAuth(const std::string &username, bool auth)
{
    usersAuth[username] = auth;
}

void Users::setUserAuthVec(const std::string &username, const std::vector<bool> &authVec)
{
    usersAuthVec[username] = authVec;
}

void Users::setUserAuthVec(const std::string &username,int autVecIndex ,bool boolean)
{
    usersAuthVec[username][autVecIndex] = boolean;
}

void Users::updateUserFd(const std::string& oldUsername, const std::string& newUsername)
{
    std::map<std::string, int>::iterator it = usersFd.find(oldUsername);
    if (it != usersFd.end())
    {
        int fd = it->second;
        usersFd.erase(it);
        usersFd.insert(std::make_pair(newUsername, fd));
    }
}

void Users::updateUserAuth(const std::string& oldUsername, const std::string& newUsername)
{
    std::map<std::string, bool>::iterator it = usersAuth.find(oldUsername);
    if (it != usersAuth.end())
    {
        bool auth = it->second;
        usersAuth.erase(it);
        usersAuth.insert(std::make_pair(newUsername, auth));
    }
}

void Users::updateUserAuthVec(const std::string& oldUsername, const std::string& newUsername)
{
    std::map<std::string, std::vector<bool> >::iterator it = usersAuthVec.find(oldUsername);
    if (it != usersAuthVec.end())
    {
        std::vector<bool>& authVec = it->second;
        usersAuthVec.erase(it);
        usersAuthVec.insert(make_pair(newUsername,authVec));
    }
}

bool Users::getUserPassAuth(const std::string& _username) const
{
    std::map<std::string, bool>::const_iterator it = usersPassAuth.find(_username);
    if (it != usersPassAuth.end())
    {
        return it->second;
    }
    return(false);
}

void Users::setUserPassAuth(const std::string& _username, bool _auth)
{
    usersPassAuth[_username] = _auth;
}

// update user pass auth
void Users::updateUserPassAuth(const std::string& oldUsername, const std::string& newUsername)
{
    std::map<std::string, bool>::iterator it = usersPassAuth.find(oldUsername);
    if (it != usersPassAuth.end())
    {
        bool auth = it->second;
        usersPassAuth.erase(it);
        usersPassAuth.insert(std::make_pair(newUsername, auth));
    }
}
