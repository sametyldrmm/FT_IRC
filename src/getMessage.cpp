#include "../inc/getMessage.hpp"

getMessage::getMessage(const std::string& str,Server &server,int _fd) : message(trim(str)),info_server(server)
{
    fd = _fd;
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
    // else if(messageVector[0] == "JOIN")
    // {
        
    // }
    // else if(messageVector[0] == "USER")
    // {
    //     // std::string oldUserName = this->info_server.users.getUsername(fd);
    //     // this->info_server.users.setUserAuthVec(oldUserName,1,true);
    // }

    else if(messageVector[0] == "NICK")
    {
        // kullanıcıya ait tüm isimleri kendi değişkenlerimde değiştir
        // ESKİ İSİM VE YENİ İSİM AYNI İSE ERROR VERDİR
        // ERRORLUK BİR DURUM YOK İSE CHANNELDA VAR İSE O ZAMAN BİR MESAJ GÖNDERMEK GEREKİR
        // JOİN YAPTIKTAN SONRA DÖN
        std::string oldUserName = this->info_server.users.getUsername(fd);
        if(messageVector[1] == oldUserName)
        {
            // bu noktada bu kısım olduğu gibi check kısmı olmalı
            //:samet-ABRA-A5-V16-6 433 KA1 KA1 :Nickname is already in use
            // BU MESAJA BİRŞEYLER EKLENMESİ GEREKİYOR
            // returnMessage = ERR_NICKNAMEINUSE(oldUserName);
            // send(socket, message.c_str(), message.length(), 0);
            send(this->info_server.users.getUserFd(oldUserName),"ERROR: AYNİ İSİM",20,0);
        }
        else
        {
            this->info_server.channels.renameUserInChannels(oldUserName,messageVector[1]);
            this->info_server.users.setUserName(fd,messageVector[1]);
            this->info_server.users.updateUserFd(oldUserName,messageVector[1]);
            this->info_server.users.updateUserAuth(oldUserName,messageVector[1]);
            this->info_server.users.updateUserAuthVec(oldUserName,messageVector[1]);
        }
    }
    else if(messageVector[0] == "PASS")
    {
        std::string oldUserName = this->info_server.users.getUsername(fd);
        // this->info_server.users.setUserAuthVec(oldUserName,2,true);
        this->info_server.users.setUserAuth(oldUserName,true);
    }
    else if(info_server.users.getUserAuth(info_server.users.getUsername(fd)) == false)
    {
        returnMessage = ERR_NOTREGISTERED(this->info_server.users.getNickname(fd), this->info_server.users.getHostname(fd)) + "\r\n";

        // returnMessage = std::string("") + ERR_NOTREGISTERED(this->info_server.users.getNickname(fd).c_str(),this->info_server.users.getHostname(fd).c_str()) + "\r\n" ;
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
