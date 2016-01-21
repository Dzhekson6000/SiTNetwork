#ifndef HTTPSOCKETCLIENT_H
#define HTTPSOCKETCLIENT_H
#include "Network/Socket.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

namespace SiTNetwork
{
    class HttpSocketClient: public Socket
    {
    public:
        HttpSocketClient();
        HttpSocketClient(int port);
        HttpSocketClient(const char *host, int port);
        virtual ~HttpSocketClient();
        
        void read(HttpResponse& httpResponse);
        void send(const HttpRequest& httpRequest);
    private:
    };
}

#endif /* HTTPSOCKETCLIENT_H */

