#ifndef HTTPSOCKET_H
#define HTTPSOCKET_H
#include "Network/Socket.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

namespace SiTNetwork
{
    class HttpSocketServer: public Socket
    {
    public:
	HttpSocketServer();
	HttpSocketServer(int port);
	HttpSocketServer(const char *host, int port);
	virtual	~HttpSocketServer();

	bool	read(HttpRequest& httpRequest);
	bool	send(const HttpResponse& httpResponse);
    private:
    } ;
}

#endif /* HTTPSOCKET_H */

