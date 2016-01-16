#include "Network/Server/ServerClient.h"
#include <thread>

using namespace SiTNetwork;

ServerClient::ServerClient(Socket socketClient):
_socketClient(socketClient)
{
    std::thread(&ServerClient::run, this).detach();
}

ServerClient::~ServerClient()
{
}

void* ServerClient::run(void* thisPtr)
{
    ((ServerClient*) thisPtr)->thread();
    delete ((ServerClient*) thisPtr);
    return nullptr;
}

void ServerClient::thread()
{
}