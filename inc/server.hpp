#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h> // Eklenen satır
#include "getMessage.hpp"
#include "Channel.hpp"
#include "Users.hpp"
#include "utils.hpp"
#include "numeric.hpp"

class getMessage ;
class Server {
public:
    Server(int port,int password);
    void start();
    Users users;
    Channel channels;
    int Password;
		
	void channelAllChangeUserInfoPush(int fd);
	void channelChangeUserInfoPush(int fd, std::string channelName);
	void channelAllChangeUserInfoPush(int fd, std::string message);
	void channelChangeUserInfoPush(int fd, std::string channelName, std::string message);

	void quitAll(int fd);
    std::map<int, std::string> clients;
    std::vector<struct pollfd> pollFds;
	void concatenateStrings(int fd,std::vector<int> users,const char* arg1, ...) ;
private:
    int port;

	int serverSocket;
    std::string hostName;

    void setHostName();
    int createSocket();
    void bindSocket(int socket);
    void listenForConnections(int socket);
    void handleNewConnection(int serverSocket);
    void handleClientSocket(int clientSocket);
    void handleClientMessage(int clientSocket);
    void printReceivedMessage(int clientSocket, const std::string& message);
    void sendMessageToClients(int senderSocket, const std::string& message);
    void closeClientSocket(int clientSocket);
    void handlePollEvents();
    void setupPolling();
    int pollSockets();
    void polError(int pollSocket);

    std::string getClientAddressString(const sockaddr_in &clientAddress);
    int getClientPort(const sockaddr_in &clientAddress);
    void addClient(int clientSocket, const std::string &clientAddress);
    void addClientToPolling(int clientSocket);

};

#endif // SERVER_HPP
