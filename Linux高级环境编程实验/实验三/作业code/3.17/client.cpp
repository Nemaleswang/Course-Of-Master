#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

template<typename ConnectionPorcessor>
class CTCPClient : public ConnectionPorcessor
{
public:
    CTCPClient(int nServerPort, const char *strServerIP)
    {
	m_nServerPort = nServerPort;

	int nlength = strlen(strServerIP);
	m_strServerIP = new char [nlength + 1];
	strcpy(m_strServerIP, strServerIP);
    }

    virtual ~CTCPClient()
    {
	delete [] m_strServerIP;
    }

public:
    int Run()
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
	if(::inet_pton(AF_INET, m_strServerIP, &ServerAddress.sin_addr) != 1)
	{
	    std::cout << "inet_pton error" << std::endl;
	    ::close(nClientSocket);
	    return -1;
	}

	ServerAddress.sin_port = htons(m_nServerPort);

	if(::connect(nClientSocket, (sockaddr*)&ServerAddress, sizeof(ServerAddress)) == -1)
	{
	    std::cout << "connect error" << std::endl;
	    ::close(nClientSocket);
	    return -1;
	}

	ConnectionPorcessor *pProcess = static_cast<ConnectionPorcessor *>(this);
	pProcess->ClientFunction(nClientSocket);

	::close(nClientSocket);

	return 0;
    }

private:
    int m_nServerPort;
    char *m_strServerIP;
};

class CMyTCPClient
{
public:
    CMyTCPClient()
    {
    }

    virtual ~CMyTCPClient()
    {
    }

    void ClientFunction(int nConnectedSocket)
    {
	char buf[13];

	::read(nConnectedSocket, buf, 13);

	std::cout << buf << std::endl;
    }
};

int main()
{
    CTCPClient<CMyTCPClient> client(4000, "127.0.0.1");
    client.Run();

    return 0;
}

