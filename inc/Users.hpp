#ifndef USERS_HPP
#define USERS_HPP

#include "include.hpp"
#include <map>
#include <string>
#include <vector>

class Users {
private:
    std::vector<int> allUsersFd;
    std::map<int, std::string> username;
    std::map<int, std::string> nickname;
    std::map<int, std::string> hostname;
    std::map<int, std::string> realname;
    std::vector<std::string> channels;
    std::map<std::string, int> usersFd;
    std::map<std::string, bool> usersAuth;
    std::map<std::string, std::vector<bool> > usersAuthVec;
    std::map<std::string, bool> usersPassAuth;

    int userCount;

public:
    Users();
    ~Users();

    void createUser(int fd,const std::string& username,const std::string& nick ,const std::string& hostname,const std::string& realName);
    // Getters
    bool getUserPassAuth(const std::string& username) const;
    std::string getUsername(int id) const;
    std::string getNickname(int id) const;
    std::string getHostname(int id) const;
    std::string getRealname(int id) const;
    std::vector<std::string> getChannels() const;
    int getUserFd(const std::string& username) const;
    bool getUserAuth(const std::string& username) const;
    std::vector<bool> getUserAuthVec(const std::string& username) const;
    int getAllUsersFd(int id) const;
    // Setters
    void setUserName(int id, const std::string& name);
    void setNickName(int id, const std::string& name);
    void setHostName(int id, const std::string& host);
    void setRealName(int id, const std::string& name);
    void addChannel(const std::string& channel);
    void removeChannel(const std::string& channel);
    void setUserFd(const std::string& username, int fd);
    void setUserAuth(const std::string& username, bool auth);
    void setUserAuthVec(const std::string& username, const std::vector<bool>& authVec);
    void setAllUserFd(int value);
    void setUserAuthVec(const std::string &username,int autVecIndex ,bool boolean);
    void setUserPassAuth(const std::string& username, bool auth);

    void updateUserFd(const std::string& oldUsername, const std::string& newUsername);
    void updateUserAuth(const std::string& oldUsername, const std::string& newUsername);
    void updateUserAuthVec(const std::string& oldUsername, const std::string& newUsername);
    void updateUserPassAuth(const std::string& oldUsername, const std::string& newUsername);

};
// kullanıcı çıkış işlemleri için remove fonksiyonları gelmeli
#endif  // USERS_HPP
