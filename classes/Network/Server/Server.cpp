#include "Server.h"
#include "../Socket.h"
#include <thread>

using namespace doxyCraft;

Server::Server():
_isUseSSL(false),
_port(0),
_isClose(false),
_onLogFunc(nullptr)
{
}

Server::Server(int port):
_isUseSSL(false),
_port(port),
_isClose(false),
_onLogFunc(nullptr)
{
}

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
    socket.setUseSSL(_isUseSSL);
    socket.setTypeProtocol(Socket::TYPE_PROTOCOL::TCP);
    socket.setTypeSocket(Socket::TYPE_SOCKET::SERVER);
    socket.create();

    Socket* socketClient = nullptr;

    while(!_isClose)
    {
	socketClient = new Socket();
	if(socketClient->accept(socket))
	    newClient(socketClient);
	else
	    delete socketClient;
    }
}

void Server::stop()
{
    _isClose = true;
}

void Server::log(String message)
{
    if(_onLogFunc)_onLogFunc(message);
}

void Server::newClient(Socket* socketClient)
{
    if(_onNewClientFunc)
	_onNewClientFunc(socketClient);
}

void Server::setNewClientFunc(OnNewClientFunc onNewClientFunc)
{
    _onNewClientFunc = onNewClientFunc;
}

void Server::setLogFunc(OnLogFunc onLogFunc)
{
    _onLogFunc = onLogFunc;
}

void Server::setUseSSL(bool isUseSSL)
{
    _isUseSSL = isUseSSL;
}

bool Server::getUseSSL()
{
    return _isUseSSL;
}
