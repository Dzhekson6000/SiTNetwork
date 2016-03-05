#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <openssl/crypto.h>

#ifdef WINDOWS
#include <winsock2.h> 
#pragma comment(lib, "WS2_32")   
#else
#include <arpa/inet.h>
typedef int SOCKET;
#define SOCKET_ERROR -1
#endif

#define DEFAULT_PORT 8000
#define DEFAULT_LISTEN_LEN 10

namespace SiTNetwork
{

    class Socket
    {
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
        
        bool create();
        void close();
        bool accept(const Socket& socket);
        SOCKET getSocket() const;
        
        void setSocket(SOCKET socket);
        
        void setUseSSL(bool isUseSSL);
        bool getUseSSL() const;
        
        void setTypeSocket(TYPE_SOCKET type_socket);
        void setTypeProtocol(TYPE_PROTOCOL type_protocol);
        
        void setHost(const char *host);
        
        ssize_t send(const void * buffer, size_t n, int flags);
        ssize_t read(void * buffer, size_t n, int flags);
        bool    sendFile(std::string path);
    private:
        struct sockaddr_in _socketaddr;
        SOCKET _socket;
        const char * _host;
        int _port;
        bool _isUseSSL;
        
        TYPE_SOCKET _type_socket;
        TYPE_PROTOCOL _type_protocol;
        
        void createAddres();
        unsigned long getHostAddress(const char* host);
        
        bool initializeSSL();
        void destroySSL();
        SSL_CTX* _ctx;
        SSL*     _ssl;
    };
}

#endif /* SOCKET_H */

