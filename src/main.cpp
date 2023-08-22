#include "../inc/server.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port>" << " <password>" << std::endl;
        return 1;
    }

    int port = atoi(argv[1]);
	if (port < 1024 || port > 65535)
	{
		std::cerr << "Port number must be between 1024 and 65535" << std::endl;
		return 1;
	}
	int password = atoi(argv[2]);
    if (password < 0 || password > 9999)
	{
		std::cerr << "Password must be between 0 and 9999" << std::endl;
		return 2;
	}
	Server server(port,password);
    server.start();

    return 0;
}
