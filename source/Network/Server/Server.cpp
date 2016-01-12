#include "Network/Server/Server.h"
#include "Network/Socket.h"

using namespace SiTNetwork;

Server::Server():
_port(0),
_onLogFunc(nullptr),
_onLogErrorFunc(nullptr),
_isClose(false)
{}

Server::Server(int port):
_port(port),
_onLogFunc(nullptr),
_onLogErrorFunc(nullptr),
_isClose(false)
{}

Server::~Server()
{
}

void* Server::run(void* thisPtr)
{
    ((Server*) thisPtr)->start();
    return nullptr;
}

void Server::start()
{
    Socket socket(8000);
    socket.setTypeProtocol(Socket::TYPE_PROTOCOL::TCP);
    socket.setTypeSocket(Socket::TYPE_SOCKET::SERVER);
    if(!socket.create())
    {
        logError(socket.getLog());
        return;
    }
    
    int clientSocket;
    struct sockaddr_in clientAddr;
    socklen_t len;
    char buffer[INET_ADDRSTRLEN];
    
    while(!_isClose)
    {
        len = sizeof(clientAddr);
        clientSocket = accept(socket.getSocket(), (struct sockaddr*)&clientAddr, &len);
        if(clientSocket < 0)
        {
            logError("Accept error");
            return;
        }
        newClient(clientSocket);
    }
}

void Server::stop()
{
    _isClose = true;
}

void Server::log(std::string message)
{
    if(_onLogFunc)_onLogFunc(message);
}

void Server::logError(std::string message)
{
    if(_onLogErrorFunc)_onLogErrorFunc(message);
}

void Server::newClient(int clientSocket)
{
    if(_onNewClientFunc)
        _onNewClientFunc(clientSocket);
    else
        new ServerClient(clientSocket);
}

void Server::setNewClientFunc(OnNewClientFunc onNewClientFunc)
{
    _onNewClientFunc = onNewClientFunc;
}

void Server::setLogFunc(OnLogFunc onLogFunc)
{
    _onLogFunc = onLogFunc;
}

void Server::setLogErrorFunc(OnLogErrorFunc onLogErrorFunc)
{
    _onLogErrorFunc = onLogErrorFunc;
}