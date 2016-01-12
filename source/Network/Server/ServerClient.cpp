#include "Network/Server/ServerClient.h"

ServerClient::ServerClient(int clientSocket):
_clientSocket(clientSocket)
{
    pthread_create(&_tid, NULL, ServerClient::run, this);//if != 0 to error
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