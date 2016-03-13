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
    char buffer[1025];
    int result;
    std::ostringstream request;

    result = Socket::read(buffer, 1024, 0);
    buffer[1024]='\0';
    if (result == SOCKET_ERROR)
    {
	close();
	return false;
    }
    else if (result > 0)
    {
	request << buffer;
    }

    return httpRequest.parse(request.str());
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
