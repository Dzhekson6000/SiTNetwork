#include "Http/HttpSocketClient.h"
#include <sstream>

using namespace SiTNetwork;

HttpSocketClient::HttpSocketClient()
{
}

HttpSocketClient::HttpSocketClient(int port):Socket(port)
{
}

HttpSocketClient::HttpSocketClient(const char* host, int port):Socket(host,port)
{
}

HttpSocketClient::~HttpSocketClient()
{
}

void HttpSocketClient::read(HttpResponse& httpResponse)
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
    
    httpResponse.parse(request.str());
}

void HttpSocketClient::send(const HttpRequest& httpRequest)
{
    if (::send(
            getSocket(), 
            httpRequest.getHttp()->c_str(),
            httpRequest.getHttp()->length(),
            0) == SOCKET_ERROR
        )
    {
        throw RuntimeError("send failed");
    }
}

