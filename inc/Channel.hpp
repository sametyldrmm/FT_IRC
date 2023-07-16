#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "include.hpp"

class Channel
{
private:
    std::vector<std::string> allChannels;
    std::map<std::string,std::vector<std::string> > channelUsers;
    std::map<std::string,std::vector<std::string> > channelAdmins;
    std::map<std::string,std::string > channelTopics;
    std::map<std::string,std::string > channelOwner;

public:
    Channel();
    ~Channel();
};

#endif  // CHANNEL_HPP