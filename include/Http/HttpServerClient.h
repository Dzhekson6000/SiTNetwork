#ifndef HTTPSERVERCLIENT_H
#define HTTPSERVERCLIENT_H
#include "Network/Server/ServerClient.h"
#include "Http/HttpRequest.h"

namespace SiTNetwork
{
    class HttpServerClient: public ServerClient
    {
    public:
	HttpServerClient(Socket* socketClient);
	virtual		~HttpServerClient();
    protected:
	void		thread();
	virtual bool	cameData(const HttpRequest& httpRequest);
    } ;
}
#endif /* HTTPSERVERCLIENT_H */

