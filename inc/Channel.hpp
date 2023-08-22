#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "include.hpp"

class Channel
{
private:
    std::vector<std::string> allChannels;
    std::map<std::string, std::vector<int> > channelUsers;
    std::map<std::string, std::string> channelTopics;
    std::map<std::string, int> channelOwner;

public:
    Channel();
    ~Channel();

    // Getter functions
    std::vector<std::string> getAllChannels() const;
    bool getAllChannels(const std::string& channel) const;
    std::vector<int> getChannelUsers(const std::string& channel) const;
    bool getChannelUsers(const std::string& channel,int fd) const;
    std::string getChannelTopic(const std::string& channel) const;
    int getChannelOwner(const std::string& channel) const;

    void allChannelsPushBack(const std::string& channel);
    void allChannelsremove(const std::string& channel);

    // Setter functions
    void setAllChannels(const std::vector<std::string>& channels);
    void setChannelUsers(const std::string& channel, const std::vector<int>& users);
    void setChannelTopic(const std::string& channel, const std::string& topic);
    void setChannelOwner(const std::string& channel, const int owner);
    // Update functions
    void updateChannelUsers(const std::string& channel, const std::vector<int> users);
    // void updateChannelAdmins(const std::string& channel, const std::vector<int> admins);
    void updateChannelTopic(const std::string& channel, const std::string& topic);
    void updateChannelOwner(const std::string& channel, int owner);

    // Additional update functions for modifying vector values
    void updateChannelUser(const std::string& channel, const int oldUser, const int newUser);
    void removeChannelUser(const std::string& channel, int fd);

    // Additional update functions for modifying vector values
    void addChannelUser(const std::string& channel, int fd);

    std::vector<std::string> findChannelsByUser(int fd) const;
	
	// RemoveChannelUsers
	bool removeChannelOwner(const std::string& channel);
	bool removeChannelAllChannel(const std::string& channel);
	bool removeChannelUsers(const std::string& channel);
	bool removeChannelTopic(const std::string& channel);
	bool removeChannelAllInfo(const std::string& channel);
};

#endif  // CHANNEL_HPP