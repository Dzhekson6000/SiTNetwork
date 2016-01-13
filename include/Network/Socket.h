#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include "RuntimeError.h"

#ifdef WINDOWS
#include <winsock2.h> 
#pragma comment(lib, "WS2_32")   
#else
#include <arpa/inet.h>
typedef int SOCKET;
#endif

#define DEFAULT_PORT 8000
#define DEFAULT_LISTEN_LEN 10

namespace SiTNetwork
{

    class Socket {
    public:
        enum TYPE_SOCKET
        {
            CLIENT,
            SERVER
        };
        
        enum TYPE_PROTOCOL
        {
            TCP,
            UDP
        };
        
        Socket();
        Socket(int port);
        Socket(const char *host, int port);
        virtual ~Socket();
        
        void create() throw(RuntimeError);
        SOCKET getSocket();
        
        void setTypeSocket(TYPE_SOCKET type_socket);
        void setTypeProtocol(TYPE_PROTOCOL type_protocol);
        
        void setHost(const char *host);
    private:
        struct sockaddr_in _socketaddr;
        SOCKET _socket;
        const char * _host;
        int _port;
        
        TYPE_SOCKET _type_socket;
        TYPE_PROTOCOL _type_protocol;
        
        void createAddres();
        unsigned long getHostAddress(const char* host);
    };
}

#endif /* SOCKET_H */

