#include <iostream>
#include <string>
#include <vector>
#include <map>

class MyClass {
private:
    std::string data;
    std::map<std::string,std::vector<std::string> > channelUsers;

public:
    MyClass(const std::string& str) : data(str) {}

    std::string operator=(std::string str ) {
        data = str;
        return "asd";
    }

    void printData() {
        std::cout << "Data: " << data << std::endl;
    }
};

int main() {
    MyClass obj("Initial value");
    obj.printData();

    std::string newValue = "New value";
    std::string assignedValue = MyClass("Initial value") = newValue;

    obj.printData();
    std::cout << "Assigned value: " << assignedValue << std::endl;

    return 0;
}