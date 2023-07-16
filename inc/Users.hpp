#ifndef USERS_HPP
#define USERS_HPP
#include "include.hpp"

class Users
{
private:
	std::string username;
	std::string nickname;
	std::string hostname;
	std::string realname;
    std::map<std::string, bool>  authMap;
    std::vector<std::string> channels;
    int fd;
    bool auth;
public:
    Users();
    ~Users();
};


#endif  // USERS_HPP