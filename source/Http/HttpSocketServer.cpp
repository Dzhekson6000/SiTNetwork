#include "Http/HttpSocketServer.h"
#include <sstream>

using namespace SiTNetwork;

HttpSocketServer::HttpSocketServer()
{
    setTypeProtocol(Socket::TYPE_PROTOCOL::TCP);
    setTypeSocket(Socket::TYPE_SOCKET::SERVER);
}

HttpSocketServer::HttpSocketServer(int port):Socket(port)
{
    setTypeProtocol(Socket::TYPE_PROTOCOL::TCP);
    setTypeSocket(Socket::TYPE_SOCKET::SERVER);
}

HttpSocketServer::HttpSocketServer(const char* host, int port):Socket(host,port)
{
    setTypeProtocol(Socket::TYPE_PROTOCOL::TCP);
    setTypeSocket(Socket::TYPE_SOCKET::SERVER);
}

HttpSocketServer::~HttpSocketServer()
{
}

void HttpSocketServer::read(HttpRequest& httpRequest)
{
    char buffer[1025];
    int result;
    std::ostringstream request;
    
    result = recv(getSocket(), buffer, 1024, 0);
    buffer[1024]='\0';
    if (result == SOCKET_ERROR)
    {
        close();
        throw RuntimeError("read failed");
    } else if (result > 0)
    {
        request << buffer;
    }
    
    httpRequest.parse(request.str());
}

void HttpSocketServer::send(const HttpResponse& httpResponse)
{
    if (::send(
            getSocket(), 
            httpResponse.getHttp()->c_str(),
            httpResponse.getHttp()->length(),
            0) == SOCKET_ERROR
        )
    {
        throw RuntimeError("send failed");
    }
}
