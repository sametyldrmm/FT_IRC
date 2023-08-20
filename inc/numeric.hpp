#ifndef NUMERIC_HPP
#define NUMERIC_HPP


#include <string>


# define VERSION "v1.0.0"

// RPL
#define RPL_WELCOME(hostname, nickName, nick, user, host)        (std::string(":") + hostname + " 001 " + nickName + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host)
#define RPL_YOURHOST(hostname, nickName, nick, host)             (std::string(":") + hostname + " 002 " + nickName + " :Your host is " + host + ", running version " + VERSION)
#define RPL_CREATED(hostname, nickName, nick, date)              (std::string(":") + hostname + " 003 " + nickName + " :This server was created " + date)
#define RPL_NOTOPIC(hostname, nickName, nick, channel)           (std::string(":") + hostname + " 331 " + nickName + " " + channel + " :No topic is set")
#define RPL_TOPIC(hostname, nickName, nick, channel, topic)      (std::string(":") + hostname + " 332 " + nickName + " " + channel + " :" + topic)
#define RPL_NAMEREPLY(hostname, nickName, nick, channel, users)  (std::string(":") + hostname + " 353 " + nickName + " = " + channel + " :" + users)
#define RPL_ENDOFNAMES(hostname, nickName, nick, channel)        (std::string(":") + hostname + " 366 " + nickName + " " + channel + " :End of /NAMES list")

#define ERR_NOSUCHNICK(hostname, nickName, nick)                 (std::string(":") + hostname + " 401 " + nickName + " " + nick + " :No such nick/channel")
#define ERR_NOSUCHCHANNEL(hostname, nickName, channel)           (std::string(":") + hostname + " 403 " + nickName + " " + channel + " :No such channel")
#define ERR_UNKNOWNCOMMAND(hostname, nickName, cmd)              (std::string(":") + hostname + " 421 " + nickName + " " + cmd + " :Unknown command")
#define ERR_NONICKNAMEGIVEN(hostname, nickName)                  (std::string(":") + hostname + " 431 " + nickName + " :No nickname given")
#define ERR_ERRONEUSNICKNAME(hostname, nickName, nick)           (std::string(":") + hostname + " 432 " + nickName + " " + nick + " :Erroneus nickname")
#define ERR_NICKNAMEINUSE(hostname, nickName)                    (std::string(":") + hostname + " 433 " + nickName + " :Nickname is already in use")
#define ERR_USERNOTINCHANNEL(hostname, nickName, nick, channel)  (std::string(":") + hostname + " 441 " + nickName + " " + nick + " " + channel + " :They aren't on that channel")
#define ERR_NOTONCHANNEL(hostname, nickName, channel)            (std::string(":") + hostname + " 442 " + nickName + " " + channel + " :You're not on that channel")
#define ERR_USERONCHANNEL(hostname, nickName, nick, channel)     (std::string(":") + hostname + " 443 " + nickName + " " + nick + " " + channel + " :is already on channel")
#define ERR_NOTREGISTERED(hostname, nickName)                    (std::string(":") + hostname + " 451 " + nickName + " :You have not registered")
#define ERR_NEEDMOREPARAMS(hostname, nickName, cmd)              (std::string(":") + hostname + " 461 " + nickName + " " + cmd + " :Not enough parameters")
#define ERR_ALREADYREGISTRED(hostname, nickName)                 (std::string(":") + hostname + " 462 " + nickName + " :Unauthorized command (already registered)")
#define ERR_PASSWDMISMATCH(hostname, nickName)                   (std::string(":") + hostname + " 464 " + nickName + " :Password incorrect")
#define ERR_CHANOPRIVSNEEDED(hostname, nickName, channel)        (std::string(":") + hostname + " 482 " + nickName + " " + channel + " :You're not channel operator")

#endif // NUMERIC_HPP END FILE
