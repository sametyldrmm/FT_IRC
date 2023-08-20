
#include "../inc/Error.hpp"

template <typename T>
T StringToNumber(const std::string& Text)
{
    std::istringstream ss(Text);
    T result;
    return ss >> result ? result : 0;
}

template <typename T>
std::string IntToString(T value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

Error::Error(const std::string& str, Server& server, int fd) : command(str), server(server), fd(fd)
{
    
}

Error::Error(const std::string& str, Server& server, int fd, const std::string &send_message) : command(str), server(server), fd(fd), send_message(send_message)
{
    
}

Error::~Error()
{
    // Destructor implementation
}

void Error::nickError()
{
    std::string ret_str;
    
    if (message == "")
    {
        this->ret_mesagge = ERR_NONICKNAMEGIVEN(this->server.users.getHostname(fd), this->server.users.getNickname(fd)) +"\r\n";
        return ;
    }
    if (message.size() > 30)
    {
        this->ret_mesagge = ERR_ERRONEUSNICKNAME(this->server.users.getHostname(fd), this->server.users.getNickname(fd), message) +"\r\n";
        return ;
    }
    if (message.find_first_of(" \t\r\n\v\f") != std::string::npos)
    {
        this->ret_mesagge = ERR_ERRONEUSNICKNAME(this->server.users.getHostname(fd), this->server.users.getNickname(fd), message) +"\r\n"; 
        return ;
    }
    if (message.find_first_not_of(VALIDCHARS) != std::string::npos)
    {
        this->ret_mesagge = ERR_ERRONEUSNICKNAME(this->server.users.getHostname(fd), this->server.users.getNickname(fd), message) +"\r\n";
        return ;
    }
    if(1)
    {
        // std::cout << message ;
        // std::vector<int> allFd = this->server.users.getAllUsersFd();
        // for(size_t i = 0; i < allFd.size(); i++)
        // {
        //     std::cout << this->server.users.getNickname(allFd[i]) << message << std::endl;
        //     if(this->server.users.getNickname(allFd[i]) == trim(message))
        //     {
        //         this->ret_mesagge = ERR_NICKNAMEINUSE(this->server.users.getHostname(fd), trim(message)) +"\r\n";
        //         return ;
        //     }
        // }
    }
    return ;
}

void Error::passError()
{
    if (message[0] != ':')
    {
        ret_mesagge = ERR_NEEDMOREPARAMS(this->server.users.getHostname(fd), this->server.users.getNickname(fd), message) +"\r\n";
        return;
    }
    if (StringToNumber<int>(message.substr(1)) != this->server.Password)
    {
        ret_mesagge = ERR_PASSWDMISMATCH(this->server.users.getHostname(fd), this->server.users.getNickname(fd)) +"\r\n";
        return;
    }
}

void Error::userError()
{
    std::string modeString;
    std::string username;
    std::string hostname;
    std::string realName;
    int mode;
    std::vector<std::string> mesgVec = split(message,' ');
    try
    {
        username = mesgVec[0];
    }
    catch (std::exception& e)
    {
        ret_mesagge = ERR_NEEDMOREPARAMS(this->server.users.getHostname(fd), this->server.users.getNickname(fd), message) +"\r\n";
        // numeric::sendNumeric(ERR_NEEDMOREPARAMS(command), user, server);
        return;
    }
    try
    {
        modeString = mesgVec[1];
    }
    catch (std::exception& e)
    {
        ret_mesagge = ERR_NEEDMOREPARAMS(this->server.users.getHostname(fd), this->server.users.getNickname(fd), message) +"\r\n";
        return;
    }

    try
    {
        mode = StringToNumber<int>(modeString);
        (void)mode;
    }
    catch (std::exception& e)
    {
        ret_mesagge = "ERROR: Invalid mode \r\n";
        return;
    }
    try
    {
        hostname = mesgVec[2];
    }
    catch (std::exception& e)
    {
        ret_mesagge = ERR_NEEDMOREPARAMS(this->server.users.getHostname(fd), this->server.users.getNickname(fd), message)+"\r\n";
        return;
    }
    try
    {
        realName = mesgVec[3].substr(3);
    }
    catch (std::exception& e)
    {
        ret_mesagge = ERR_NEEDMOREPARAMS(this->server.users.getHostname(fd), this->server.users.getNickname(fd), message)+"\r\n";
        return;
    }
    // this->server.users.updateUserNameAllValue(this->server.users.getUsername(fd), username);
    this->server.users.setUserName(fd,username);
    this->server.users.setUserMod(fd, mode);
    this->server.users.setHostName(fd, hostname);
    this->server.users.setRealName(fd, realName);
}

void Error::joinError()
{
    if (message == "")
	{
        ret_mesagge = ERR_NEEDMOREPARAMS(this->server.users.getHostname(fd), this->server.users.getNickname(fd), message)+"\r\n";
        return;
	}
    if(split(message,' ').size() > 2)
    {
        ret_mesagge = ERR_NEEDMOREPARAMS(this->server.users.getHostname(fd), this->server.users.getNickname(fd), message)+"\r\n";
        return;
    }
    if(split(message,' ')[0][0] != '#')
    {
        ret_mesagge = ERR_NOSUCHCHANNEL(this->server.users.getHostname(fd), this->server.users.getNickname(fd), message)+"\r\n";
        return;
    }
    if(message.size() > 50)
    {
        ret_mesagge = ERR_NOSUCHCHANNEL(this->server.users.getHostname(fd), this->server.users.getNickname(fd), message)+"\r\n";
        return;
    }
}

void Error::privMsg()
{
    std::vector<std::string> msgVector = split(message,' ');
    if(msgVector[0][0] == '#')
    {
        if(!this->server.channels.getAllChannels(msgVector[0].substr(1)))
            ERR_NOSUCHCHANNEL(this->server.users.getHostname(fd),this->server.users.getNickname(fd),msgVector[0])+"\r\n";
    }
    else
    {
        std::vector<int> allUserFd = this->server.users.getAllUsersFd();
        for(size_t i = 0; i < allUserFd.size(); i++)
        {
            if( msgVector[0] == this->server.users.getNickname(allUserFd[i]))
                return;
        }
        ret_mesagge = ERR_NOSUCHNICK(this->server.users.getHostname(fd),this->server.users.getNickname(fd),msgVector[0]) + "\r\n";
    }
    return ;
}

void Error::partError()
{
    if (message == "" )
	{
        ret_mesagge = ERR_NEEDMOREPARAMS(this->server.users.getHostname(fd),this->server.users.getNickname(fd),"PART")+ "\r\n";
	}
    std::vector<std::string> msgVec = split(message,' ');
	if (this->server.channels.getAllChannels(msgVec[0].substr(1)) == false)
	{
        ret_mesagge = ERR_NOSUCHCHANNEL(this->server.users.getHostname(fd),this->server.users.getNickname(fd),msgVec[0]) +"\r\n";
	}
	if (this->server.channels.getChannelUsers(msgVec[0].substr(1),fd) == false)
	{
        ret_mesagge = ERR_NOTONCHANNEL(this->server.users.getHostname(fd),this->server.users.getNickname(fd),msgVec[0]) +"\r\n";
	}
	
	int tempp_fd = this->server.channels.getChannelOwner(msgVec[0].substr(1));
	
	std::cout << tempp_fd << "<a" << fd << "\n;";

	if (fd == tempp_fd)
	{
		std::vector<int> users = this->server.channels.getChannelUsers(msgVec[0].substr(1));
		if (users.size() == 1)
		{
			std::string server_msg = this->server.users.getPrefix(fd) + " PART " + msgVec[0] + " " + send_message + "\r\n";
            sender(fd, server_msg);
			this->server.channels.removeChannelAllInfo(msgVec[0].substr(1));
			ret_mesagge = " ";
			return ;
		}
		else
		{
	
			int temp_fd = users[fd == users[0]];
			std::cout << "fd: " << fd << "    temp_fd: " << temp_fd << std::endl;
			this->server.channels.updateChannelOwner(msgVec[0].substr(1), users[0 + fd == users[0]]);
			sender(temp_fd, (this->server.users.getPrefix(temp_fd) + " MODE " + msgVec[0] + " +o " + this->server.users.getNickname(temp_fd) + "\r\n"));
			
			return ;
		}
	}



    return ;
}

void Error::kickError()
{
	if (message == "")
	{
		ret_mesagge = ERR_NEEDMOREPARAMS(this->server.users.getHostname(fd),this->server.users.getNickname(fd),"KICK") +"\r\n";
		return ;
	}
	std::cout << "message : " << message << std::endl;
	std::vector<std::string> msgVec = split(message,' ');
	
	if (this->server.channels.getAllChannels(msgVec[0].substr(1)) == false)
	{
		ret_mesagge = ERR_NOSUCHCHANNEL(this->server.users.getHostname(fd),this->server.users.getNickname(fd),msgVec[0]) +"\r\n";
		return ;

	}
	if (msgVec.size() < 2)
	{
		ret_mesagge = ERR_NOSUCHNICK(this->server.users.getHostname(fd),this->server.users.getNickname(fd),msgVec[1]) +"\r\n";
		return ;
	}

	int user_fd = this->server.users.getNickname(msgVec[1]);
	if (this->server.channels.getChannelUsers(msgVec[0].substr(1), user_fd) == false)
	{
		ret_mesagge = ERR_NOTONCHANNEL(this->server.users.getHostname(fd),this->server.users.getNickname(fd),msgVec[0]) +"\r\n";
		return ;
	}

	if (fd != this->server.channels.getChannelOwner(msgVec[0].substr(1)))
	{
		ret_mesagge = ERR_CHANOPRIVSNEEDED(this->server.users.getHostname(fd),this->server.users.getNickname(fd),msgVec[0]) +"\r\n";
		return ;
	}

	if (user_fd == this->server.channels.getChannelOwner(msgVec[0].substr(1)))
	{
		std::vector<int> users = this->server.channels.getChannelUsers(msgVec[0].substr(1));
		if (users.size() == 1)
		{
			std::cout << "users_fd[i] : " << this->server.users.getNickname(msgVec[1]) << std::endl;
			std::string server_msg = this->server.users.getPrefix(this->server.users.getNickname(msgVec[1])) + " KICK " + msgVec[0] + " " + msgVec[1] + "\r\n";
			sender(this->server.users.getNickname(msgVec[1]), server_msg);
			this->server.channels.removeChannelAllInfo(msgVec[0].substr(1));
			ret_mesagge = " ";
			return ;
		}
		else
		{
			if (user_fd != users[0 + user_fd == users[0]])
			{
				int temp_fd = users[0 + user_fd == users[0]];
				this->server.channels.updateChannelOwner(msgVec[0].substr(1), users[0 + user_fd == users[0]]);
				sender(fd, (this->server.users.getPrefix(temp_fd) + " MODE " + msgVec[0] + " +o " + this->server.users.getNickname(temp_fd) + "\r\n"));
				return ;
			}
			return ;
		}
	}

}
//

std::string Error::operator=(const std::string& str)
{
    this->message = str;
    if(command == "NICK")
    {
        nickError();
    }
    if(command == "PASS")
    {
        passError();
    }
    if(command == "USER")
    {
        userError();
    }
    if(command == "JOIN")
    {
        joinError();
    }
    if(command == "PRIVMSG")
    {
        privMsg();   
    }
    if(command == "PART")
    {
        partError();
    }
	if(command == "KICK")
	{
		kickError();
	}

    return ret_mesagge;
}

