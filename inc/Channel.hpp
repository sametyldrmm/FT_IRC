#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "include.hpp"

class Channel
{
private:
    std::vector<std::string> allChannels;
    std::map<std::string, std::vector<std::string> > channelUsers;
    std::map<std::string, std::vector<std::string> > channelAdmins;
    std::map<std::string, std::string> channelTopics;
    std::map<std::string, std::string> channelOwner;

public:
    Channel();
    ~Channel();

    // Getter functions
    std::vector<std::string> getAllChannels() const;
    std::vector<std::string> getChannelUsers(const std::string& channel) const;
    std::vector<std::string> getChannelAdmins(const std::string& channel) const;
    std::string getChannelTopic(const std::string& channel) const;
    std::string getChannelOwner(const std::string& channel) const;

    // Setter functions
    void setAllChannels(const std::vector<std::string>& channels);
    void setChannelUsers(const std::string& channel, const std::vector<std::string>& users);
    void setChannelAdmins(const std::string& channel, const std::vector<std::string>& admins);
    void setChannelTopic(const std::string& channel, const std::string& topic);
    void setChannelOwner(const std::string& channel, const std::string& owner);

    // Update functions
    void updateChannelUsers(const std::string& channel, const std::vector<std::string>& users);
    void updateChannelAdmins(const std::string& channel, const std::vector<std::string>& admins);
    void updateChannelTopic(const std::string& channel, const std::string& topic);
    void updateChannelOwner(const std::string& channel, const std::string& owner);

    // Additional update functions for modifying vector values
    void updateChannelUser(const std::string& channel, const std::string& oldUser, const std::string& newUser);
    void updateChannelAdmin(const std::string& channel, const std::string& oldAdmin, const std::string& newAdmin);
    void removeChannelUser(const std::string& channel, const std::string& user);
    void removeChannelAdmin(const std::string& channel, const std::string& admin);

    // Additional update functions for modifying vector values
    void addChannelUser(const std::string& channel, const std::string& user);
    void addChannelAdmin(const std::string& channel, const std::string& admin);

    std::vector<std::string> findChannelsByUser(const std::string& user) const;
    void renameUserInChannels( const std::string& oldUserName, const std::string& newUserName);

};

#endif  // CHANNEL_HPP