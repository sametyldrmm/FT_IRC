#ifndef USERS_HPP
#define USERS_HPP

#include "include.hpp"
#include <map>
#include <string>
#include <vector>

struct UsersAuth {
    bool userPass;
    bool userNick;
    bool userUser;
    UsersAuth(bool pass = false, bool nick = false, bool user = false)
        : userPass(pass), userNick(nick), userUser(user)
    {
    }
};

class Users {
private:
    std::vector<int> allUsersFd;
    std::map<int, std::string> username;
    std::map<int, std::string> nickname;
    std::map<int, std::string> hostname;
    std::map<int, std::string> realname;
    std::map<int, int> usersMod;
    std::vector<std::string> channels;
    std::map<int, bool> usersAuth; 
    std::map<int, UsersAuth> usersAuthStruct;
    std::map<int, bool> usersWelcomeMessage;

public:
    Users();
    ~Users();

    void createUser(int fd, const std::string& username, const std::string& nick, const std::string& hostname, const std::string& realName);

    // Getters for Username
    std::string getUsername(int id) const;
    void setUserName(int id, const std::string& name);

    // Getters for Nickname
    std::string getNickname(int id) const;
	int getNickname(const std::string& nick) const;
    void setNickName(int id, const std::string& name);
	bool controlNickName(const std::string name);
	void removeUserAllInfo(int fd);

    // Getters for Hostname
    std::string getHostname(int id) const;
    void setHostName(int id, const std::string& host);

    // Getters for Realname
    std::string getRealname(int id) const;
    void setRealName(int id, const std::string& name);

    // getters setters updates for channels
    std::vector<std::string> getChannels() const;
    void addChannel(const std::string& channel);
    void removeChannel(const std::string& channel);

    // Getters Setters and Updates for allUsersFd
    void addAllUserFd(int fd);
    int getAllUsersFd(int id) const;
    std::vector<int> getAllUsersFd() const;

    // Getters Setters and Updates for userAuth
    bool getUserAuth(int fd) const;
    void setUserAuth(int fd, bool auth);
    void autoUpdateUserAuth(int fd);

    // Getters Setters and Updates for userAuthStruct
    void setUserAuthStruct(int fd, struct UsersAuth auth);
    struct UsersAuth getUserAuthStruct(int fd) const;
    void setUserAuthStruct(int fd, const std::string& header, bool auth);
    bool getUserAuthStruct(int fd, const std::string& header) const;
    // Getters Setters and Updates for 

    // Getters Setters and Updates for userWelcomeMessage
    void setUserWelcomeMessage(int fd, bool auth);
    bool getUserWelcomeMessage(int fd) const;

    // Getters Setters and Updates for mod
    void setUserMod(int id, int mod);
    int getUserMod(int id) const;

	std::string getPrefix(const int &fd) const;

};

#endif //USERS_HPP