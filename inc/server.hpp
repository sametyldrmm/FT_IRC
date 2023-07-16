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

class Server {
public:
    Server(int port);
    void start();

private:
    int port;
    int serverSocket;
    std::vector<struct pollfd> pollFds;
    std::map<int, std::string> clients;
    std::string hostName;

    Users users;
    Channel channels;

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
