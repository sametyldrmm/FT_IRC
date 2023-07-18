#include"../inc/Channel.hpp"


Channel::Channel()
{
    // Constructor implementation
}

Channel::~Channel()
{
    // Destructor implementation
}

// Getter functions

std::vector<std::string> Channel::getAllChannels() const
{
    return allChannels;
}

std::vector<std::string> Channel::getChannelUsers(const std::string& channel) const
{
    if (channelUsers.count(channel) > 0)
    {
        return channelUsers.at(channel);
    }
    return std::vector<std::string>(); // Return an empty vector if channel not found
}

std::vector<std::string> Channel::getChannelAdmins(const std::string& channel) const
{
    if (channelAdmins.count(channel) > 0)
    {
        return channelAdmins.at(channel);
    }
    return std::vector<std::string>(); // Return an empty vector if channel not found
}

std::string Channel::getChannelTopic(const std::string& channel) const
{
    if (channelTopics.count(channel) > 0)
    {
        return channelTopics.at(channel);
    }
    return ""; // Return an empty string if channel not found
}

std::string Channel::getChannelOwner(const std::string& channel) const
{
    if (channelOwner.count(channel) > 0)
    {
        return channelOwner.at(channel);
    }
    return ""; // Return an empty string if channel not found
}

// Setter functions

void Channel::setAllChannels(const std::vector<std::string>& channels)
{
    allChannels = channels;
}

void Channel::setChannelUsers(const std::string& channel, const std::vector<std::string>& users)
{
    channelUsers[channel] = users;
}

void Channel::setChannelAdmins(const std::string& channel, const std::vector<std::string>& admins)
{
    channelAdmins[channel] = admins;
}

void Channel::setChannelTopic(const std::string& channel, const std::string& topic)
{
    channelTopics[channel] = topic;
}

void Channel::setChannelOwner(const std::string& channel, const std::string& owner)
{
    channelOwner[channel] = owner;
}

// Update functions

void Channel::updateChannelUsers(const std::string& channel, const std::vector<std::string>& users)
{
    if (channelUsers.count(channel) > 0)
    {
        channelUsers[channel] = users;
    }
}

void Channel::updateChannelAdmins(const std::string& channel, const std::vector<std::string>& admins)
{
    if (channelAdmins.count(channel) > 0)
    {
        channelAdmins[channel] = admins;
    }
}

void Channel::updateChannelTopic(const std::string& channel, const std::string& topic)
{
    if (channelTopics.count(channel) > 0)
    {
        channelTopics[channel] = topic;
    }
}

void Channel::updateChannelOwner(const std::string& channel, const std::string& owner)
{
    if (channelOwner.count(channel) > 0)
    {
        channelOwner[channel] = owner;
    }
}

// Additional update functions for modifying vector values

void Channel::removeChannelUser(const std::string& channel, const std::string& user)
{
    if (channelUsers.count(channel) > 0)
    {
        std::vector<std::string>& users = channelUsers[channel];
        users.erase(std::remove(users.begin(), users.end(), user), users.end());
    }
}

void Channel::updateChannelUser(const std::string& channel, const std::string& oldUser, const std::string& newUser)
{
    if (channelUsers.count(channel) > 0)
    {
        std::vector<std::string>& users = channelUsers[channel];
        std::vector<std::string>::iterator it;
        for (it = users.begin(); it != users.end(); ++it)
        {
            if (*it == oldUser)
            {
                *it = newUser;
                break;
            }
        }
    }
}

void Channel::updateChannelAdmin(const std::string& channel, const std::string& oldAdmin, const std::string& newAdmin)
{
    if (channelAdmins.count(channel) > 0)
    {
        std::vector<std::string>& admins = channelAdmins[channel];
        std::vector<std::string>::iterator it;
        for (it = admins.begin(); it != admins.end(); ++it)
        {
            if (*it == oldAdmin)
            {
                *it = newAdmin;
                break;
            }
        }
    }
}

void Channel::removeChannelAdmin(const std::string& channel, const std::string& admin)
{
    if (channelAdmins.count(channel) > 0)
    {
        std::vector<std::string>& admins = channelAdmins[channel];
        admins.erase(std::remove(admins.begin(), admins.end(), admin), admins.end());
    }
}

// Additional update functions for modifying vector values

void Channel::addChannelUser(const std::string& channel, const std::string& user)
{
    channelUsers[channel].push_back(user);
}

void Channel::addChannelAdmin(const std::string& channel, const std::string& admin)
{
    channelAdmins[channel].push_back(admin);
}

std::vector<std::string> Channel::findChannelsByUser(const std::string& user) const
{
    std::vector<std::string> matchingChannels;
    
    for (std::map<std::string, std::vector<std::string> >::const_iterator it = channelUsers.begin(); it != channelUsers.end(); ++it)
    {
        const std::vector<std::string>& users = it->second;
        if (std::find(users.begin(), users.end(), user) != users.end())
        {
            matchingChannels.push_back(it->first);
        }
    }
    
    for (std::map<std::string, std::vector<std::string> >::const_iterator it = channelAdmins.begin(); it != channelAdmins.end(); ++it)
    {
        const std::vector<std::string>& admins = it->second;
        if (std::find(admins.begin(), admins.end(), user) != admins.end())
        {
            if (std::find(matchingChannels.begin(), matchingChannels.end(), it->first) == matchingChannels.end())
            {
                matchingChannels.push_back(it->first);
            }
        }
    }
    
    return matchingChannels;
}

void Channel::renameUserInChannels(const std::string& oldUserName, const std::string& newUserName)
{
    std::vector<std::string> channelNames = findChannelsByUser(oldUserName);
    for (std::vector<std::string>::const_iterator it = channelNames.begin(); it != channelNames.end(); ++it)
    {
        std::string channelName = *it;
        updateChannelUser(channelName, oldUserName, newUserName);
        updateChannelAdmin(channelName, oldUserName, newUserName);
    }
}
