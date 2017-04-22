#include "ServerClient.h"
#include "../Socket.h"
#include <thread>

using namespace doxyCraft;

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