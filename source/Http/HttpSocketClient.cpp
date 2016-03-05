#include "Http/HttpSocketClient.h"
#include <sstream>
#include <string.h>

using namespace SiTNetwork;

HttpSocketClient::HttpSocketClient()
{
    setTypeProtocol(Socket::TYPE_PROTOCOL::TCP);
    setTypeSocket(Socket::TYPE_SOCKET::CLIENT);
}

HttpSocketClient::HttpSocketClient(int port):Socket(port)
{
    setTypeProtocol(Socket::TYPE_PROTOCOL::TCP);
    setTypeSocket(Socket::TYPE_SOCKET::CLIENT);
}

HttpSocketClient::HttpSocketClient(const char* host, int port):Socket(host,port)
{
    setTypeProtocol(Socket::TYPE_PROTOCOL::TCP);
    setTypeSocket(Socket::TYPE_SOCKET::CLIENT);
}

HttpSocketClient::~HttpSocketClient()
{
}

bool HttpSocketClient::read(HttpResponse& httpResponse)
{
    char buffer[1025];
    int result;
    std::ostringstream request;
    
    while(true)
    {
        result = Socket::read(buffer, 1024, 0);
        buffer[1024]='\0';
        if (result == SOCKET_ERROR)
        {
            close();
            return false;
        } else if (result > 0)
        {
            request << buffer;
        } else if (result == 0)
        {
            break;
        }
        memset(buffer, 0, sizeof(buffer));
    }
    
    return httpResponse.parse(request.str());
}

bool HttpSocketClient::send(const HttpRequest& httpRequest)
{
    if (Socket::send( 
            httpRequest.getHttp()->c_str(),
            httpRequest.getHttp()->length(),
            0) == SOCKET_ERROR
        )
    {
        return false;
    }
    return true;
}

