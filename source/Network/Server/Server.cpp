#include "Network/Server/Server.h"
#include <sys/socket.h>
#include <unistd.h>

Server::Server():
_port(DEFAULT_PORT),
_onLogFunc(nullptr),
_onLogErrorFunc(nullptr),
_isClose(false),
_requestsCount(0)
{}

Server::Server(int port):
_port(port),
_onLogFunc(nullptr),
_onLogErrorFunc(nullptr),
_isClose(false),
_requestsCount(0)
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
    const int on = 1;
    
    _socket = socket(AF_INET , SOCK_STREAM , 0);
    
    if (_socket == -1)
    {
        logError("Could not create socket");
        return;
    }
    log("Socket created");
    
    bzero(&_server, sizeof(_server));
    _server.sin_family = AF_INET;
    _server.sin_addr.s_addr = INADDR_ANY;
    _server.sin_port = htons(_port);
    
    setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    
    if( bind(_socket,(struct sockaddr*)&_server , sizeof(_server)) < 0)
    {
        logError("bind failed. Error");
        return;
    }
    log("bind done");
    
    
    if (listen(_socket , DEFAULT_LISTEN_LEN) < 0 ) {
        logError("listen. Error");
        return;
    }
    
    int clientSocket;
    
    struct sockaddr_in clientAddr;
    socklen_t len;
    char buffer[INET_ADDRSTRLEN];
    
    while(!_isClose)
    {
        len = sizeof(clientAddr);
        clientSocket = accept(_socket, (struct sockaddr*)&clientAddr, &len);
        if(clientSocket < 0)
        {
            logError("Accept error");
            return;
        }
        newClient(clientSocket);
        _requestsCount++;
        
        
    }
    close(_socket);
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