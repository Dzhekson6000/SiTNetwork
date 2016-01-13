#include "Network/Server/ServerClient.h"
#include <thread>

ServerClient::ServerClient(int clientSocket):
_clientSocket(clientSocket)
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