#include <iostream>
#include <cstdarg>
#include <string>

// Değişken sayıda string argümanları birleştiren işlev
std::string concatenateStrings(int fd,const char* arg1, ...) {
    std::string result;
    va_list args;
    va_start(args, arg1);

    const char* currentArg = arg1;
    while (currentArg != NULL) {
        result += currentArg;
        currentArg = va_arg(args, const char*);
    }

    va_end(args);
	std::cout << fd;
    return result;
}

int main() {
    std::string str1 = "Merhaba";
    std::string str2 = " ";
    std::string str3 = "Dünya";
    std::string str4 = " ";
    std::string str5 = "Nasılsınız?";

    std::string result = concatenateStrings(3,str1.c_str(), str2.c_str(), str3.c_str(), str4.c_str(), str5.c_str(), NULL);
    std::cout << result << std::endl;
    
    return 0;
}
