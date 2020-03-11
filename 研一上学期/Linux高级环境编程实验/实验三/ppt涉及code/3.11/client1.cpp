#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>

int main()
{
    int nClientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == nClientSocket)
    {
	std::cout << "socket error" << std::endl;
	return 0;
    }

    sockaddr_in ServerAddress;
    memset(&ServerAddress, 0, sizeof(sockaddr_in));
    ServerAddress.sin_family = AF_INET;
    if(::inet_pton(AF_INET, "127.0.0.1", &ServerAddress.sin_addr) != 1)
    {
	std::cout << "inet_pton error" << std::endl;
	return 0;
    }

    ServerAddress.sin_port = htons(2345);

    if(::connect(nClientSocket, (sockaddr*)&ServerAddress, sizeof(ServerAddress)) == -1)
    {
	std::cout << "connect error" << std::endl;

	std::cout << "errno is " << errno << std::endl;

	return 0;
    }

    char buf[13];
    ::read(nClientSocket, buf, 13);

    std::cout << buf << std::endl;

    ::close(nClientSocket);

    return 0;
}

