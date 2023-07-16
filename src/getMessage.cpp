#include "../inc/getMessage.hpp"

getMessage::getMessage(const std::string& str) : message(trim(str)) 
{
    messageVector = split(message.c_str(),' ');
    typeFinder();
}

getMessage::~getMessage() {}

void getMessage::typeFinder()
{
    std::vector<std::string> messageVector = split(message,' ');
    if (messageVector[0] == "CAP")
    {
        if(messageVector[1] == "LS")
            returnMessage = senderCapLs();
        if(messageVector[1] == "ACK")
            returnMessage = senderCapAck();
    }
}

std::string getMessage::senderCapLs()
{
    std::string capLs = "CAP * LS :multi-prefix sasl\r\n";
    return capLs;
}

std::string getMessage::senderCapAck()
{
    std::string capAck = "CAP * ACK :multi-prefix\r\n";
    return capAck;
}

std::string getMessage::operator=(const std::string& str )
{
    free = str;
    return returnMessage;
}
