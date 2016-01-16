#include "Network/Server/Server.h"

using namespace SiTNetwork;

Server::Server():
_port(0),
_onLogFunc(nullptr),
_isClose(false)
{}

Server::Server(int port):
_port(port),
_onLogFunc(nullptr),
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
    Socket socket(_port);
    socket.setTypeProtocol(Socket::TYPE_PROTOCOL::TCP);
    socket.setTypeSocket(Socket::TYPE_SOCKET::SERVER);
    socket.create();
    
    Socket socketClient = Socket();
    
    while(!_isClose)
    {
        socketClient.accept(socket);
        newClient(socketClient);
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

void Server::newClient(Socket socketClient)
{
    if(_onNewClientFunc)
        _onNewClientFunc(socketClient);
    else
        new ServerClient(socketClient);
}

void Server::setNewClientFunc(OnNewClientFunc onNewClientFunc)
{
    _onNewClientFunc = onNewClientFunc;
}

void Server::setLogFunc(OnLogFunc onLogFunc)
{
    _onLogFunc = onLogFunc;
}