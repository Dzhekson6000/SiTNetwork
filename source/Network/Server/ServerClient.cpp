#include "Network/Server/ServerClient.h"
#include <thread>

using namespace SiTNetwork;

ServerClient::ServerClient(Socket* socketClient):
_socketClient(socketClient)
{
}

ServerClient::~ServerClient()
{
    if(_socketClient)
    {
	_socketClient->close();
	delete _socketClient;
    }
}

void ServerClient::thread()
{
}