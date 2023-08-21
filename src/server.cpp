#include "../inc/server.hpp"
#include <cerrno>
#include <errno.h>
#include "../inc/numeric.hpp"

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

struct PollFdFinder
{
    int clientSocket;
    PollFdFinder(int socket) : clientSocket(socket) {}

    bool operator()(const struct pollfd &pollFd)
    {
        return pollFd.fd == clientSocket;
    }
};

Server::Server(int port) : port(port), serverSocket(0) 
{
    Password = 123;
}

void Server::polError(int pollSocket)
{
    if (pollSocket == -1)
    {
        std::cerr << "Error: Failed to poll sockets." << std::endl;
        exit(1);
    }
}

void Server::start()
{
    serverSocket = createSocket();
    bindSocket(serverSocket);
    listenForConnections(serverSocket);
    setHostName();
    std::cout << "Waiting for connections on port " << port << std::endl;

    setupPolling();

    while (true)
    {
        polError(pollSockets());
        handlePollEvents();
    }
}

void Server::setupPolling()
{
    pollFds.resize(1);
    pollFds[0].fd = serverSocket;
    pollFds[0].events = POLLIN;
}

int Server::pollSockets()
{
    return poll(&pollFds[0], pollFds.size(), -1);
}

void Server::handlePollEvents()
{
    for (size_t i = 0; i < pollFds.size(); i++)
    {
        if (pollFds[i].revents & POLLIN)
        {
            if (pollFds[i].fd == serverSocket)
            {
                handleNewConnection(serverSocket); // yeni bağlantı varsa
            }
            else
            {
                handleClientSocket(pollFds[i].fd); // yeni bağlantı yoksa mesaj gelmiştir
            }
        }
    }
}

int Server::createSocket()
{
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd == -1)
    {
        std::cerr << "Error: Failed to create socket." << std::endl;
        exit(1);
    }
    return socketFd;
}

void Server::bindSocket(int socket)
{
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(socket, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cerr << "Error: Failed to bind socket." << std::endl;
        exit(1);
    }
}

void Server::listenForConnections(int socket)
{
    if (listen(socket, SOMAXCONN) == -1)
    {
        std::cerr << "Error: Failed to listen on socket." << std::endl;
        exit(1);
    }
}

void Server::handleNewConnection(int serverSocket)
{
    sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (sockaddr *)&clientAddress, &clientAddressSize);

    if (clientSocket == -1)
    {
        std::cerr << "Error: Failed to accept connection." << std::endl;
        return;
    }

    std::string clientAddressStr = getClientAddressString(clientAddress);
    int clientPort = getClientPort(clientAddress);
    std::cout << "Connected to client: " << clientAddressStr << " (Port: " << clientPort << ")" << std::endl;

    addClient(clientSocket, clientAddressStr);
    addClientToPolling(clientSocket);
}

std::string Server::getClientAddressString(const sockaddr_in &clientAddress)
{
    return inet_ntoa(clientAddress.sin_addr);
}

int Server::getClientPort(const sockaddr_in &clientAddress)
{
    return ntohs(clientAddress.sin_port);
}

void Server::addClient(int clientSocket, const std::string &clientAddress)
{
    clients.insert(std::make_pair(clientSocket, clientAddress));
}

void Server::addClientToPolling(int clientSocket)
{
    struct pollfd newClientPollFd;
    newClientPollFd.fd = clientSocket;
    newClientPollFd.events = POLLIN;
    std::string undefined = std::string("undefined") +  IntToString(newClientPollFd.fd);
    users.createUser(newClientPollFd.fd,undefined,undefined,undefined,undefined);
	pollFds.push_back(newClientPollFd);
	// NOT REGİSTERED
	static int i = 0;
	// if(i++ != 0)
	// 	sender(clientSocket, ERR_NOTREGISTERED(this->hostName,undefined) + "\r\n") ;

}

void Server::handleClientSocket(int clientSocket)
{
    struct pollfd pollFd;
    pollFd.fd = clientSocket;
    pollFd.events = POLLIN;

    if (poll(&pollFd, 1, 0) == -1)
    {
        std::cerr << "Error: Failed to poll client socket." << std::endl;
        return;
    }

    if (pollFd.revents & POLLIN)
    {
        handleClientMessage(clientSocket);
    }
}

void Server::handleClientMessage(int clientSocket)
{
    char buffer[256];
    int numBytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (numBytes <= 0)
    {
        closeClientSocket(clientSocket);
    }
    else
    {
        buffer[numBytes] = '\0';
        std::string message = buffer;
        std::cout << message << std::endl;
        printReceivedMessage(clientSocket, message); // geln mesajı cevapla
        sendMessageToClients(clientSocket, message); // herkese gönder şimdilik var
    }
}

void Server::printReceivedMessage(int clientSocket, const std::string &message)
{
    std::cout << "Received message from client " << clientSocket << ": " << message << std::endl;
    // gelen mesaj CAP LS ise
    // bu mesajları burada almak mantıksız böyle çirkin if else yapmak yerine bir class oluştururum o class constructorunda string gelen mesajı alırım işlemleri içerisinde yaparım
    std::string capLs = getMessage(message,*this,clientSocket) = message;
    if(capLs.size() > 5)
    {
        sender(clientSocket, capLs.c_str());
    }
}

void Server::sendMessageToClients(int senderSocket, const std::string &message)
{
    (void)senderSocket;
    (void)message;
    // for (size_t i = 0; i < pollFds.size(); i++)
    // {
    //     int socket = pollFds[i].fd;
    //     if (socket != serverSocket && socket != senderSocket)
    //     {
    //         sender(socket, message.c_str());
    //     }
    // }
}

void Server::closeClientSocket(int clientSocket)
{
    std::cout << "Client " << clientSocket << " disconnected." << std::endl;
    close(clientSocket);

    std::map<int, std::string>::iterator it = clients.find(clientSocket);
    if (it != clients.end())
    {
        clients.erase(it);
    }

    std::vector<struct pollfd>::iterator pollIt = std::find_if(pollFds.begin(), pollFds.end(), PollFdFinder(clientSocket));
    if (pollIt != pollFds.end())
    {
        pollFds.erase(pollIt);
    }
}

void Server::setHostName()
{
    char hostname[2048];
    int error = gethostname(hostname, 2048);
    if(error != 0)
    {
        std::cerr << "Error: Failed to get hostname."<< std::strerror(errno) << std::endl;
        exit(1);
    }
    hostName = hostname;
}

void Server::channelChangeUserInfoPush(int fd, std::string channelName)
{
    std::string all_users = "";
    int own_fd = this->channels.getChannelOwner(channelName);
    all_users += this->users.getNickname(own_fd) + " ";
	std::vector<int> channelUsers = this->channels.getChannelUsers(channelName);
    for (int i = 0; i < (int)channelUsers.size(); i++)
    {
        if (own_fd != channelUsers[i])
            all_users += this->users.getNickname(channelUsers[i]) + " ";
    }
	all_users = trim(all_users);
    for (int i = 0; i < (int)channelUsers.size(); i++)
    {
        fd = channelUsers[i];
        std::string _353 = RPL_NAMEREPLY( this->users.getHostname(fd), this->users.getNickname(fd), this->users.getNickname(fd), "#" + channelName, "@" + all_users) + "\r\n";
        std::string _356 = RPL_ENDOFNAMES(this->users.getHostname(fd), this->users.getNickname(fd), this->users.getNickname(fd), "#" + channelName) + "\r\n";
        sender(fd, _353.c_str());
        // sender(fd, getMessage::senderCapLs().c_str());
        sender(fd, _356.c_str());
    }
}

void Server::channelAllChangeUserInfoPush(int fd)
{
	std::vector<std::string> userChannels = this->channels.findChannelsByUser(fd);

	for (size_t i = 0; i < userChannels.size(); i++)
	{
		channelChangeUserInfoPush(fd, userChannels[i]);
		std::cout << "channel name" << userChannels[i] << " ";
	}
	
}



void Server::channelChangeUserInfoPush(int fd, std::string channelName, std::string message)
{
    std::string all_users = "";
    int own_fd = this->channels.getChannelOwner(channelName);
    all_users += this->users.getNickname(own_fd) + " ";
	std::vector<int> channelUsers = this->channels.getChannelUsers(channelName);

    for (int i = 0; i < (int)channelUsers.size(); i++)
    {
        fd = channelUsers[i];
        sender(fd, message);
    }
}

void Server::channelAllChangeUserInfoPush(int fd, std::string message)
{
	std::vector<std::string> userChannels = this->channels.findChannelsByUser(fd);

	for (size_t i = 0; i < userChannels.size(); i++)
	{
		channelChangeUserInfoPush(fd, userChannels[i], message);
		std::cout << "channel name" << userChannels[i] << " ";
	}
}

void Server::quitAll(int fd)
{
	std::vector<std::string> userChannels = this->channels.findChannelsByUser(fd);

	for (int i = 0; i < (int)userChannels.size() ; i++)
	{
		if (userChannels[i].size() == 1)
		{
			this->channels.removeChannelAllInfo(userChannels[i]);

			std::string server_msg = this->users.getPrefix(fd) + " PART " + userChannels[i] + " " + "\r\n";
            sender(fd, server_msg);
			continue;
		}
		if (this->channels.getChannelOwner(userChannels[i]) == fd)
		{
			std::vector<int> users = this->channels.getChannelUsers(userChannels[i]);
			int temp_fd = users[fd == users[0]];
			this->channels.updateChannelOwner(userChannels[i] , users[fd == users[0]]);
			sender(fd, (this->users.getPrefix(temp_fd) + " MODE #" + userChannels[i] + " +o " + this->users.getNickname(temp_fd) + "\r\n"));

			std::vector<int> channelUsers = this->channels.getChannelUsers(userChannels[i]);
            for (int i = 0; i < (int)channelUsers.size(); i++)
            {
                std::string server_msg = this->users.getPrefix(fd) + " PART " + userChannels[i] + " " + "\r\n";
                sender(channelUsers[i], server_msg);
            }

			this->channels.removeChannelUser(userChannels[i], fd);
			this->users.removeUserAllInfo(fd);
			continue;
		}
		else
		{
			this->channels.removeChannelUser(userChannels[i], fd);

						std::vector<int> channelUsers = this->channels.getChannelUsers(userChannels[i]);
            for (int i = 0; i < (int)channelUsers.size(); i++)
            {
                std::string server_msg = this->users.getPrefix(fd) + " PART " + userChannels[i] + " " + "\r\n";
                sender(channelUsers[i], server_msg);
            }

			this->users.removeUserAllInfo(fd);
			continue;
		}
	}

	this->users.removeUserAllInfo(fd);
}