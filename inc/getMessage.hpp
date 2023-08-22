#ifndef GETMESSAGE_HPP
#define GETMESSAGE_HPP

#include "include.hpp"
#include "utils.hpp"
#include "server.hpp"
#include "Error.hpp"
#include "numeric.hpp"

class Server ;

class getMessage
{
private:
    std::string message;
    std::vector<std::string> messageVector;
    std::string returnMessage;
    std::string free;

    void typeFinder();
    std::string senderCapLs();
    std::string senderCapAck();
    int fd;
    Server &info_server;
public:
    std::string operator=(const std::string& str )
    {
    	(void)str;
    	return returnMessage;
	}
    void CommandCap(const std::string &joinMessage);
    void CommandNick(const std::string &joinMessage);
    void CommandUser(const std::string &joinMessage);
    void CommandPass(const std::string &joinMessage);
    void CommandJoin(const std::string &joinMessage);
    void CommandWelcome(const std::string &joinMessage);
    void CommandError(const std::string &joinMessage);
    void CommandPrivMsg(const std::string &joinMessage);
    void CommandPart(const std::string &joinMessage);
    void CommandKick(const std::string &joinMessage);
    void CommandQuit(const std::string &joinMessage);
    void CommandTopic(const std::string &joinMessage);
	void CommandNotice(const std::string &joinMessage);
    getMessage(const std::string& str,Server &server,int fd);
    ~getMessage();
};
// std::string getMessage::operator=(const std::string& str)

#endif  // GETMESSAGE_HPP
