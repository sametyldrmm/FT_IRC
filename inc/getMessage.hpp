#ifndef GETMESSAGE_HPP
#define GETMESSAGE_HPP

#include "include.hpp"
#include "utils.hpp"
#include "server.hpp"

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
    std::string operator=(const std::string& str );
    getMessage(const std::string& str,Server &server,int fd);
    ~getMessage();
};

#endif  // GETMESSAGE_HPP