#ifndef GETMESSAGE_HPP
#define GETMESSAGE_HPP

#include "include.hpp"
#include "utils.hpp"

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

public:
    std::string operator=(const std::string& str );
    getMessage(const std::string& str);
    ~getMessage();
};

#endif  // GETMESSAGE_HPP