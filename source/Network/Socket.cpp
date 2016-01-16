#include "Network/Socket.h"
#include <string.h>
#include <unistd.h>
#include <netdb.h>

using namespace SiTNetwork;

Socket::Socket():_socket(0), _host(nullptr),_port(DEFAULT_PORT){}

Socket::Socket(int port):_socket(0),_host(nullptr),_port(port){}

Socket::Socket(const char *host, int port):_socket(0),_host(host),_port(port){}

Socket::~Socket()
{
    close();
}

void Socket::create() throw(RuntimeError)
{
#ifdef _WIN32
    WSADATA         WsaData;  
    int err = WSAStartup (0x0101, &WsaData);
    if(err!=0)  
        throw RuntimeError();
#endif
    
    switch(_type_protocol)
    {
        case TCP:
            _socket = socket(AF_INET, SOCK_STREAM, 0);
            break;
        case UDP:
            _socket = socket(AF_INET, SOCK_DGRAM, 0);
            break;
    }
 
#ifdef _WIN32
    if (_socket == INVALID_SOCKET)
    {
        WSACleanup();
        throw RuntimeError("Could not create socket");
    }
#else
    if (_socket == -1)
    {
        throw RuntimeError("Could not create socket");
    }
#endif  
    
    
    createAddres();

    switch(_type_socket)
    {
        case SERVER:
        {
            const int on = 1;
            if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on) == -1)
            {
                throw RuntimeError("setsockopt(SO_REUSEADDR) failed");
            }

            if( bind(_socket,(struct sockaddr*)&_socketaddr , sizeof(_socketaddr)) < 0)
            {
                close();
#ifdef _WIN32
                WSACleanup();
#endif
                throw RuntimeError("bind failed. Error");
            }

            if(_type_protocol == TCP)
            {
                if (listen(_socket , DEFAULT_LISTEN_LEN) < 0 )
                {
                    close();
#ifdef _WIN32
                    WSACleanup();
#endif
                    throw RuntimeError("listen. Error");
                }
            }
        }
        break;
        case CLIENT:
        {
            if(_type_protocol == TCP)
            {
                if(connect(_socket,(sockaddr*)&_socketaddr, sizeof(_socketaddr)))  
                {
                    close();
                    throw RuntimeError("connect failed");
                }	
            }   
        }
        break;
    }
}

void Socket::close()
{
#ifdef _WIN32
    closesocket(_socket);
#else
    ::close(_socket);
#endif
}

Socket* Socket::accept(const Socket& socket)
{
    socklen_t len;
    len = sizeof(_socketaddr);
    _socket = ::accept(socket.getSocket(), (struct sockaddr*)&_socketaddr, &len);
    if(_socket < 0)
    {
        throw RuntimeError("Accept error");
    }
}

void Socket::createAddres()
{
    memset(&_socketaddr, sizeof(_socketaddr), 0);
    _socketaddr.sin_family = AF_INET;
    switch(_type_socket)
    {
        case SERVER:
            _socketaddr.sin_addr.s_addr = INADDR_ANY;
            break;
        case CLIENT:
            _socketaddr.sin_addr.s_addr = getHostAddress(_host);
            break;
    }
    
    _socketaddr.sin_port = htons(_port);
    
}

unsigned long Socket::getHostAddress(const char* host)
{
	struct hostent *phe;  
	char *p;  
	phe = gethostbyname(host);  
	if(phe==NULL) 
	{
		throw RuntimeError("failed in gethostbyname");
	}
	p = *phe->h_addr_list;  
	return *((unsigned long*)p);  
}

SOCKET Socket::getSocket() const
{
    return _socket;
}

void Socket::setSocket(SOCKET socket)
{
    _socket = socket;
}

void Socket::setHost(const char* host)
{
    _host = host;
}

void Socket::setTypeProtocol(TYPE_PROTOCOL type_protocol)
{
    _type_protocol = type_protocol;
}

void Socket::setTypeSocket(TYPE_SOCKET type_socket)
{
    _type_socket = type_socket;
}
