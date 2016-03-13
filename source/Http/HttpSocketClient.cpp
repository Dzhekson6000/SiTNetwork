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
    char* buffer = new char[_bufferSize+1];
    int result;
    
    while(httpResponse.getParseStatus()!=Http::PARSE_STATUS::PARSE_END)
    {
        result = Socket::read(buffer, _bufferSize, 0);
        buffer[result]='\0';
        if (result == SOCKET_ERROR)
        {
            close();
            return false;
        }
	else if (result > 0)
        {
            if(!httpResponse.parseNewDate(buffer))return false;
        } else if (result == 0)
        {
            break;
        }
        memset(buffer, 0, sizeof(char)*_bufferSize);
    }
    return true;
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

