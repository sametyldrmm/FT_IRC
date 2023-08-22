#ifndef ERROR_HPP
#define ERROR_HPP

#include "include.hpp"
#include "server.hpp"
#include "numeric.hpp"
#include <iostream>
#include <string>
// class oluşturucaz serveri kendi constructorında referans olarak alıcak

#define VALIDCHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`_^{|}-"
class Server;

class Error
{
private:
    std::string command;
    std::string message;
    Server& server;
    int fd;
    std::string ret_mesagge;
	std::string send_message;
public:
    Error(const std::string& str, Server& server, int fd);
	Error(const std::string& str, Server& server, int fd, const std::string &send_message);
    void nickError();
    void passError();
    void userError();
    void joinError();
    void privMsg();
    void partError();
	void kickError();
    void noticeError();
	void topicError();
    std::string operator=(const std::string& str);
    ~Error();
};

#endif // ERROR_HPP
