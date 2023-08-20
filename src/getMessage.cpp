#include "../inc/getMessage.hpp"
#include <vector>

template <typename T>
T StringToNumber(const std::string &Text)
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

getMessage::getMessage(const std::string &str, Server &server, int _fd) : message(trim(str)), info_server(server)
{
    fd = _fd;
    messageVector = split(message.c_str(), ' ');
    typeFinder();
}

getMessage::~getMessage() {}

void getMessage::CommandCap(const std::string &joinMessage)
{
    (void)joinMessage;
    if (joinMessage[0] == 'L')
        returnMessage = senderCapLs();
    if (joinMessage[0] == 'A')
        returnMessage = senderCapAck();
}

void getMessage::CommandNick(const std::string &joinMessage)
{
    std::string error = Error("NICK", this->info_server, fd) = joinMessage.c_str();

    if (error != std::string("") )
    {
        sender(fd, error);
    }
    else
    {
		if (!(this->info_server.users.controlNickName(messageVector[1])))
		{
			
			std::string temp_sendd = "ERROR : NICK ERROR!!!!!! \r\n";
			sender(fd,temp_sendd);
			return ;
		}
        this->info_server.users.setUserAuthStruct(fd, "nick", true);
        std::string prefix = ":" + this->info_server.users.getNickname(fd) + "!" + this->info_server.users.getUsername(fd) + "@" + this->info_server.users.getHostname(fd);
        std::string nickMessage = prefix + " NICK " + messageVector[1] + "\r\n";
        sender(fd, nickMessage);
        this->info_server.users.setNickName(fd, messageVector[1]);
        std::cout << "NICK MESAJI GÖNDERİLDİ" << "\n";
		this->info_server.users.autoUpdateUserAuth(fd);


		this->info_server.channelAllChangeUserInfoPush(fd, nickMessage);
		this->info_server.channelAllChangeUserInfoPush(fd);
    }
}

void getMessage::CommandUser(const std::string &joinMessage)
{
    std::string error = Error("USER", this->info_server, fd) = joinMessage.c_str();

    if (error != std::string(""))
    {
        sender(fd, error);
        sender(fd, std::string("ERROR :Closing Link: (user@host) [Client exited]\r\n"));
    }
    else
    {
        this->info_server.users.setUserAuthStruct(fd, "user", true);
        // diğer işlemleri mecburiyetten ötürü Erorr içerisinde yaptık
		this->info_server.users.autoUpdateUserAuth(fd);
    }
}

void getMessage::CommandPass(const std::string &joinMessage)
{
    std::string error = Error("PASS", this->info_server, fd) = joinMessage.c_str();

    if (error != std::string(""))
    {
        sender(fd, error);
    }
    else
    {
        this->info_server.users.setUserAuthStruct(fd, "pass", true);
		this->info_server.users.autoUpdateUserAuth(fd);
    }
}

void getMessage::CommandJoin(const std::string &joinMessage)
{
    std::string error = Error("JOIN", this->info_server, fd) = joinMessage.c_str();

    if (error != std::string(""))
    {
        sender(fd, error);
    }
    else
    {
        std::string channelName = messageVector[1].substr(1, messageVector[1].size() - 1);
        // channel yoksa channelı oluştur
        if (this->info_server.channels.getAllChannels(channelName) == false)
        {
            this->info_server.channels.allChannelsPushBack(channelName);
            this->info_server.channels.setChannelOwner(channelName, fd);
            // this->info_server.channels.setChannelAdmins(channelName, fd);
            sender(fd, (this->info_server.users.getPrefix(fd) + " JOIN " + messageVector[1] + "\r\n"));
            sender(fd, (this->info_server.users.getPrefix(fd) + " MODE " + messageVector[1] + " +o " + this->info_server.users.getNickname(fd) + "\r\n"));
            // 353 ve 356 mesaajları gönderilecek
        }
        if (this->info_server.channels.getChannelUsers(channelName, fd) == false)
        {
            this->info_server.channels.addChannelUser(channelName, fd);
            sender(fd, (this->info_server.users.getPrefix(fd) + " JOIN " + messageVector[1] + "\r\n"));
        }

        std::vector<int> channelUsers = this->info_server.channels.getChannelUsers(channelName);
		if (channelUsers.size() < 1)
		{
			this->info_server.channels.allChannelsremove(messageVector[0].substr(1));
			return ;
		}
        
		this->info_server.channelChangeUserInfoPush(fd, channelName);
    }
}

void getMessage::CommandWelcome(const std::string &joinMessage)
{
    (void)joinMessage;
    this->info_server.users.setUserWelcomeMessage(fd, true);
    std::string hostname = this->info_server.users.getHostname(fd);
    std::string nickname = this->info_server.users.getNickname(fd);
    std::string username = this->info_server.users.getUsername(fd);
    std::string realname = this->info_server.users.getRealname(fd);

    std::string welcomeMessage = RPL_WELCOME(hostname, nickname, nickname, username, hostname) + "\r\n";
    std::string yourHostMessage = RPL_YOURHOST(hostname, nickname, hostname, hostname) + "\r\n";
    std::string createdMessage = RPL_CREATED(hostname, nickname, nickname, "20.00") + "\r\n";

    sender(fd, welcomeMessage.c_str());
    sender(fd, yourHostMessage.c_str());
    sender(fd, createdMessage.c_str());
    // sender(fd, getMessage::senderCapLs().c_str());
}

void getMessage::CommandError(const std::string &joinMessage)
{
    (void)joinMessage;
    returnMessage = ERR_NOTREGISTERED(this->info_server.users.getNickname(fd), this->info_server.users.getHostname(fd)) + "\r\n";
    return;
}

void getMessage::CommandPrivMsg(const std::string &joinMessage)
{
    std::vector<std::string> joinMessageVector;
    this->messageVector = split(message, ' ');

    joinMessageVector = std::vector<std::string>(messageVector.begin() + 3, messageVector.end());
    std::string senderMessages = join(joinMessageVector, " ");

    std::string error = Error("PRIVMSG", this->info_server, fd) = joinMessage.c_str();
    if (error != std::string(""))
        sender(fd, error);
    else
    {
        if (messageVector[1][0] == '#')
        {
            std::vector<int> channelUsers = this->info_server.channels.getChannelUsers(messageVector[1].substr(1));
            for (int i = 0; i < (int)channelUsers.size(); i++)
            {
                if (fd != channelUsers[i])
                    sender(channelUsers[i], this->info_server.users.getPrefix(fd) + " PRIVMSG " + messageVector[1] + " " + messageVector[2] + "\r\n");
            }
        }
        else
        {
            std::vector<int> allUserFd = this->info_server.users.getAllUsersFd();
            for (size_t i = 0; i < allUserFd.size(); i++)
            {
                if (messageVector[1] == this->info_server.users.getNickname(allUserFd[i]))
                {
                    std::string sendMsg = this->info_server.users.getPrefix(fd) + " PRIVMSG " + messageVector[1] + " " + messageVector[2] + "\r\n";
                    sender(allUserFd[i], sendMsg);
                    return;
                }
            }
        }
    }
}

void getMessage::CommandPart(const std::string &joinMessage)
{
    std::string error = Error("PART", this->info_server, fd,"good bye") = joinMessage.c_str();

    if (error != std::string(""))
    {
        sender(fd, error);
    }
    else
    {
        std::vector<std::string> msg_vec = split(joinMessage, ':');
        std::string send_msg = msg_vec[1];
        msg_vec = split(joinMessage, ' ');
        std::cout << messageVector[0] << messageVector[1];
        if (messageVector[1][0] == '#')
        {
            std::vector<int> channelUsers = this->info_server.channels.getChannelUsers(messageVector[1].substr(1));
            for (int i = 0; i < (int)channelUsers.size(); i++)
            {
                std::string server_msg = this->info_server.users.getPrefix(fd) + " PART " + messageVector[1] + " " + send_msg + "\r\n";
                sender(channelUsers[i], server_msg);
            }
        }

		this->info_server.channels.removeChannelUser(messageVector[1].substr(1), fd);
		this->info_server.channelChangeUserInfoPush(fd, messageVector[1].substr(1));

    }
}

void getMessage::CommandKick(const std::string &joinMessage)
{
	std::string error = Error("KICK", this->info_server, fd) = joinMessage.c_str();

	if (error != std::string(""))
	{
		sender(fd, error);
	}
	else
	{
		std::vector<std::string> msgVec = split(joinMessage, ' ');
		std::vector<int> channelUsers = this->info_server.channels.getChannelUsers(msgVec[0].substr(1));
		
		for (int i = 0; i < (int)channelUsers.size(); i++)
		{
			std::cout << "users_fd[i] : " << channelUsers[i] << std::endl;
			std::string server_msg = this->info_server.users.getPrefix(channelUsers[i]) + " KICK " + msgVec[0] + " " + msgVec[1] + "\r\n";
			sender(channelUsers[i], server_msg);
		}

		this->info_server.channels.removeChannelUser(msgVec[0].substr(1), this->info_server.users.getNickname(msgVec[1]));
		this->info_server.channelChangeUserInfoPush(this->info_server.users.getNickname(msgVec[1]), msgVec[0].substr(1));
	}

}

void getMessage::CommandQuit(const std::string &joinMessage)
{
	std::string error = Error("QUIT", this->info_server, fd) = joinMessage.c_str();

	if (error != std::string(""))
	{
		sender(fd, error);
	}
	else
	{

		std::string server_msg = this->info_server.users.getPrefix(fd) + " PRIVMSG" + joinMessage.substr(1) + "\r\n";
		this->info_server.channelAllChangeUserInfoPush(fd, server_msg);
	}
}

void getMessage::typeFinder()
{
    std::vector<std::string> joinMessageVector;
    std::string joinMessage;
    this->messageVector = split(message, ' ');
    joinMessageVector = std::vector<std::string>(messageVector.begin() + 1, messageVector.end());
    joinMessage = join(joinMessageVector, " ");

    this->info_server.users.autoUpdateUserAuth(fd);

    if (messageVector[0] == "CAP")
        CommandCap(messageVector[1]);
    // else if (info_server.users.getUserAuth(fd) == false && messageVector[0] != "USER" && messageVector[0] != "NICK" && messageVector[0] != "PASS")
    //     CommandError(joinMessage);
    else if (messageVector[0] == "JOIN")
        CommandJoin(joinMessage);
    else if (messageVector[0] == "USER")
        CommandUser(joinMessage);
    else if (messageVector[0] == "NICK")
        CommandNick(joinMessage);
    else if (messageVector[0] == "PASS")
        CommandPass(joinMessage);
    else if (messageVector[0] == "PRIVMSG")
        CommandPrivMsg(joinMessage);
    else if (messageVector[0] == "PART")
        CommandPart(joinMessage);
    else if (messageVector[0] == "WHO" || messageVector[0] == "NOTICE" || messageVector[0] == "PING")
        returnMessage = "";
    else if ( messageVector[0] == "KICK")
		CommandKick(joinMessage);
	if (this->info_server.users.getUserWelcomeMessage(fd) == false && info_server.users.getUserAuth(fd))
        CommandWelcome(joinMessage);
    if (info_server.users.getUserAuth(fd) == false && !(messageVector[0] == "WHO" || messageVector[0] == "NOTICE" || messageVector[0] == "PING" || messageVector[0] == "NICK" || messageVector[0] == "PASS" || messageVector[0] == "USER"))
    {
        if (info_server.users.getUserAuthStruct(fd, "nick") == false)
            returnMessage = "NOTICE: You must send NICK before sending any command\r\n";
        else if (info_server.users.getUserAuthStruct(fd, "user") == false)
            returnMessage = "ERROR :You must send USER before sending any command\r\n";
        else if (info_server.users.getUserAuthStruct(fd, "pass") == false)
            returnMessage = "ERROR :You must send PASS before sending any command\r\n";
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
