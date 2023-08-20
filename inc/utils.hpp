#ifndef UTILS_HPP
#define UTILS_HPP

#include "include.hpp"

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string &s);
std::string rtrim(const std::string &s);
std::string trim(const std::string &s) ;
std::vector<std::string> split(const std::string& str, char delimiter); 
std::string join(const std::vector<std::string>& vec, const std::string& delimiter);
void sender(int fd,const std::string msj);


#endif // UTİLS_HPP