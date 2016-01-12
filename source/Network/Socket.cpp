#include "Network/Socket.h"
#include <string.h>
#include <unistd.h>
#include <netdb.h>

using namespace SiTNetwork;

Socket::Socket():_socket(0), _host(nullptr),_port(DEFAULT_PORT),_log(""){}

Socket::Socket(int port):_socket(0),_host(nullptr),_port(port),_log(""){}

Socket::Socket(const char *host, int port):_socket(0),_host(host),_port(port),_log(""){}

Socket::~Socket()
{
#ifdef _WIN32
	closesocket(_socket);
#else
	close(_socket);
#endif
}

bool Socket::create()
{
#ifdef _WIN32
    WSADATA         WsaData;  
    int err = WSAStartup (0x0101, &WsaData);
    if(err!=0)  
        return false;
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
#else
    if (_socket == -1)
#endif
    {
        _log = "Could not create socket";
        return false;
    }
    
    createAddres();

    switch(_type_socket)
    {
        case SERVER:
        {
            const int on = 1;
            if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on) == -1)
            {
                _log = "setsockopt(SO_REUSEADDR) failed";
                return false;
            }

            if( bind(_socket,(struct sockaddr*)&_socketaddr , sizeof(_socketaddr)) < 0)
            {
                _log = "bind failed. Error";
                return false;
            }

            if(_type_protocol == TCP)
            {
                if (listen(_socket , DEFAULT_LISTEN_LEN) < 0 )
                {
                    _log = "listen. Error";
                    return false;
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
                    _log = "connect failed";
                    return false;  
                }	
            }   
        }
        break;
    }
    _log = "OK";
    return true;
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
		_log = "failed in gethostbyname";
		return 0;  
	}
	p = *phe->h_addr_list;  
	return *((unsigned long*)p);  
}

SOCKET Socket::getSocket() {
    return _socket;
}

void Socket::setHost(const char* host) {
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

std::string Socket::getLog()
{
    return _log;
}
