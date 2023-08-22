#include "../inc/Channel.hpp"

Channel::Channel()
{
    // Constructor implementation
}

Channel::~Channel()
{
    // Destructor implementation
}

std::vector<std::string> Channel::getAllChannels() const
{
    return allChannels;
}

bool Channel::getAllChannels(const std::string& channel) const
{
    std::vector<std::string>::const_iterator it = std::find(allChannels.begin(), allChannels.end(), channel);
    if (it != allChannels.end())
    {
        return true;
    }
    return false;
}

std::vector<int> Channel::getChannelUsers(const std::string& channel) const
{
    std::vector<int> users;
    std::map<std::string, std::vector<int> >::const_iterator it = channelUsers.find(channel);
    if (it != channelUsers.end())
    {
        users = it->second;
    }
    return users;
}

bool Channel::getChannelUsers(const std::string& channel,int fd) const
{
	std::cout << "getChannelUsers" << channel << "\n";

    std::vector<int> users;
    std::map<std::string, std::vector<int> >::const_iterator it = channelUsers.find(channel);
    if (it != channelUsers.end())
    {
        users = it->second;
        std::vector<int>::const_iterator it2 = std::find(users.begin(), users.end(), fd);
        if (it2 != users.end())
        {
            return true;
        }
    }
    return false;    
}

std::string Channel::getChannelTopic(const std::string& channel) const
{
    std::map<std::string, std::string>::const_iterator it = channelTopics.find(channel);
    if (it != channelTopics.end())
    {
        return it->second;
    }
    return ""; // Return an empty string if channel not found
}

int Channel::getChannelOwner(const std::string& channel) const
{
    std::map<std::string, int>::const_iterator it = channelOwner.find(channel);
    if (it != channelOwner.end())
    {
        return it->second;
    }
    return -1; // Return -1 if channel not found
}

void Channel::allChannelsPushBack(const std::string& channel)
{
    allChannels.push_back(channel);
}

void Channel::allChannelsremove(const std::string& channel)
{
    std::vector<std::string>::iterator it = std::find(allChannels.begin(), allChannels.end(), channel);
    if (it != allChannels.end())
    {
        allChannels.erase(it);
    }
}
// Setter functions

void Channel::setAllChannels(const std::vector<std::string>& channels)
{
    allChannels = channels;
}

void Channel::setChannelUsers(const std::string& channel, const std::vector<int>& users)
{
    channelUsers[channel] = users;
}

void Channel::setChannelTopic(const std::string& channel, const std::string& topic)
{
    channelTopics[channel] = topic;
}

void Channel::setChannelOwner(const std::string& channel, int owner)
{
    channelOwner[channel] = owner;
}

// Update functions

void Channel::updateChannelUsers(const std::string& channel, const std::vector<int> users)
{
    std::map<std::string, std::vector<int> >::iterator it = channelUsers.find(channel);
    if (it != channelUsers.end())
    {
        it->second = users;
    }
}

void Channel::updateChannelTopic(const std::string& channel, const std::string& topic)
{
    std::map<std::string, std::string>::iterator it = channelTopics.find(channel);
    if (it != channelTopics.end())
    {
        it->second = topic;
    }
}

void Channel::updateChannelOwner(const std::string& channel, int owner)
{
    std::map<std::string, int>::iterator it = channelOwner.find(channel);
    if (it != channelOwner.end())
    {
        it->second = owner;
    }
}

// Additional update functions for modifying vector values

void Channel::removeChannelUser(const std::string& channel, int fd)
{
    std::map<std::string, std::vector<int> >::iterator it = channelUsers.find(channel);
    if (it != channelUsers.end())
    {
        std::vector<int>& users = it->second;
        users.erase(std::remove(users.begin(), users.end(), fd), users.end());
    }
}

void Channel::updateChannelUser(const std::string& channel, int oldUser, int newUser)
{
    std::map<std::string, std::vector<int> >::iterator it = channelUsers.find(channel);
    if (it != channelUsers.end())
    {
        std::vector<int>& users = it->second;
        for (std::vector<int>::iterator iter = users.begin(); iter != users.end(); ++iter)
        {
            if (*iter == oldUser)
            {
                *iter = newUser;
                break;
            }
        }
    }
}

// Additional update functions for modifying vector values

void Channel::addChannelUser(const std::string& channel, int fd)
{
    channelUsers[channel].push_back(fd);
}


std::vector<std::string> Channel::findChannelsByUser(int fd) const
{
    std::vector<std::string> matchingChannels;

    // Check in channelUsers
    for (std::map<std::string, std::vector<int> >::const_iterator it = channelUsers.begin(); it != channelUsers.end(); ++it)
    {
        const std::vector<int>& users = it->second;
        if (std::find(users.begin(), users.end(), fd) != users.end())
        {
            matchingChannels.push_back(it->first);
        }
    }

	for (size_t i = 0; i < matchingChannels.size(); i++)
	{
		std::cout << matchingChannels[i] << " " ;
	}
	std::cout << std::endl;
    return matchingChannels;
}

bool Channel::removeChannelOwner(const std::string& channel)
{
	std::map<std::string, int>::iterator it = channelOwner.find(channel);
	if (it != channelOwner.end())
	{
		channelOwner.erase(it);
		return true;
	}
	return false;
}

bool Channel::removeChannelAllChannel(const std::string& channel)
{
	try
	{
		this->allChannels.erase(std::find(allChannels.begin(),allChannels.end(),channel));
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return false;
	}
	return true;	
}

bool Channel::removeChannelUsers(const std::string& channel)
{
	std::map<std::string, std::vector<int> >::iterator it = channelUsers.find(channel);
	if (it != channelUsers.end())
	{
		channelUsers.erase(it);
		return true;
	}
	return false;
}

bool Channel::removeChannelTopic(const std::string& channel)
{
	std::map<std::string, std::string>::iterator it = channelTopics.find(channel);
	if (it != channelTopics.end())
	{
		channelTopics.erase(it);
		return true;
	}
	return false;
}

bool Channel::removeChannelAllInfo(const std::string& channel)
{
	bool a = removeChannelOwner(channel);
	bool b = removeChannelUsers(channel);
	bool c = removeChannelTopic(channel);
	bool d = removeChannelAllChannel(channel);
	return (a && b && c && d);
}


