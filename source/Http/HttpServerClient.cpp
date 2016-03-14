#include "Http/HttpServerClient.h"
#include "Http/HttpSocketServer.h"

using namespace SiTNetwork;

HttpServerClient::HttpServerClient(Socket* socketClient):
ServerClient(socketClient)
{
}

HttpServerClient::~HttpServerClient()
{
}

void HttpServerClient::thread()
{
    HttpRequest httpRequest;

    bool isStop = false;
    bool isKeepAlive = true;
    while(!isStop && isKeepAlive)
    {
	if(!((HttpSocketServer*)_socketClient)->read(httpRequest)) return;
	isStop = cameData(httpRequest);
	isKeepAlive = httpRequest.isKeepAlive();
    }
}

bool HttpServerClient::cameData(const HttpRequest& httpRequest)
{
    return true;
}


