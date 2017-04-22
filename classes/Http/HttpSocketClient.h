#ifndef HTTPSOCKETCLIENT_H
#define HTTPSOCKETCLIENT_H
#include "../Network/Socket.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

namespace doxyCraft
{
    class HttpSocketClient: public Socket
    {
    public:
	HttpSocketClient();
	HttpSocketClient(int port);
	HttpSocketClient(const char *host, int port);
	virtual	~HttpSocketClient();

	bool	read(HttpResponse& httpResponse);
	bool	send(const HttpRequest& httpRequest);
    private:
    } ;
}

#endif /* HTTPSOCKETCLIENT_H */

