#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>

typedef void (* TCPClient)(int nConnectedSocket);

int RunTCPClient(TCPClient ClientFunction, int nServerPort, const char *strServerIP)
{
    int nClientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == nClientSocket)
    {
	std::cout << "socket error" << std::endl;
	return -1;
    }

    sockaddr_in ServerAddress;
    memset(&ServerAddress, 0, sizeof(sockaddr_in));
    ServerAddress.sin_family = AF_INET;
    if(::inet_pton(AF_INET, strServerIP, &ServerAddress.sin_addr) != 1)
    {
	std::cout << "inet_pton error" << std::endl;
	::close(nClientSocket);
	return -1;
    }

    ServerAddress.sin_port = htons(nServerPort);

    if(::connect(nClientSocket, (sockaddr*)&ServerAddress, sizeof(ServerAddress)) == -1)
    {
	std::cout << "connect error" << std::endl;
	::close(nClientSocket);
	return -1;
    }
    
    ClientFunction(nClientSocket);
    
    ::close(nClientSocket);
    
    return 0;
}

void MyClient(int nConnectedSocket)
{
    char buf[13];
    
    ::read(nConnectedSocket, buf, 13);

    std::cout << buf << std::endl;
}

int main()
{
    RunTCPClient(MyClient, 4000, "127.0.0.1");

    return 0;
}

