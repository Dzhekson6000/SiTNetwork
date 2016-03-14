#include "Http/HttpSocketServer.h"
#include <sstream>
#include <string.h>

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

HttpSocketServer::HttpSocketServer(const char* host, int port):Socket(host, port)
{
    setTypeProtocol(Socket::TYPE_PROTOCOL::TCP);
    setTypeSocket(Socket::TYPE_SOCKET::SERVER);
}

HttpSocketServer::~HttpSocketServer()
{
}

bool HttpSocketServer::read(HttpRequest& httpRequest)
{
    char* buffer = new char[_bufferSize + 1];
    int result;
    
    while (httpRequest.getParseStatus() != Http::PARSE_STATUS::PARSE_END)
    {
	result = Socket::read(buffer, _bufferSize, 0);
	buffer[result] = '\0';
	if (result == SOCKET_ERROR)
	{
	    close();
	    return false;
	}
	else if (result > 0)
	{
	    if (!httpRequest.parseNewDate(buffer))return false;
	}
	else if (result == 0)
	{
	    if (httpRequest.getParseStatus() != Http::PARSE_STATUS::PARSE_END)
	    {
		httpRequest.endTransfer();
		httpRequest.parse();
	    }
	    break;
	}
	memset(buffer, 0, sizeof (char)*_bufferSize);
    }
    return true;
}

bool HttpSocketServer::send(const HttpResponse& httpResponse)
{
    if (Socket::send(
		     httpResponse.getHttp()->c_str(),
		     httpResponse.getHttp()->length(),
		     0) == SOCKET_ERROR
	    )
    {
	return false;
    }
    return true;
}
