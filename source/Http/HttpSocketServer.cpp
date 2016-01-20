#include "Http/HttpSocketServer.h"
#include <sstream>

using namespace SiTNetwork;

HttpSocketServer::HttpSocketServer()
{
}

HttpSocketServer::HttpSocketServer(int port):Socket(port)
{
}

HttpSocketServer::HttpSocketServer(const char* host, int port):Socket(host,port)
{
}

HttpSocketServer::~HttpSocketServer()
{
}

void HttpSocketServer::read(HttpRequest& httpRequest)
{
    char buffer[1024];
    int result;
    std::ostringstream request;
    
    result = recv(getSocket(), buffer, 1024, 0);
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
