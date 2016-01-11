#ifndef SERVER_H
#define	SERVER_H

#include <string.h>
#include <functional>
#include <arpa/inet.h>
#include "ServerClient.h"

#define DEFAULT_PORT 8000
#define DEFAULT_LISTEN_LEN 10

#define NET_CALLBACK_1(func, _Object) std::bind(&func,_Object, std::placeholders::_1)
#define NET_CALLBACK_2(func, _Object) std::bind(&func,_Object, std::placeholders::_1, std::placeholders::_2)
#define NET_CALLBACK_3(func, _Object) std::bind(&func,_Object, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

typedef std::function<void(std::string)> OnLogFunc;
typedef std::function<void(std::string)> OnLogErrorFunc;
typedef std::function<void(int clientSocket)> OnNewClientFunc;

class Server {
public:
    Server();
    Server(int port);
    virtual ~Server();
    void start();
    void stop();
    void setLogFunc(OnLogFunc onLogFunc);
    void setLogErrorFunc(OnLogErrorFunc onLogErrorFunc);
    
    void setNewClientFunc(OnNewClientFunc onNewClientFunc);
    
    static void* run(void* thisPtr);
private:
    struct sockaddr_in _server;
    
    int _port;
    int _socket;
    bool _isClose;
    static void startServer(int port);
    void log(std::string message);
    void logError(std::string message);
    void newClient(int clientSocket);

    int _requestsCount;    
    
    OnLogFunc _onLogFunc;
    OnLogErrorFunc _onLogErrorFunc;
    OnNewClientFunc _onNewClientFunc;
};

#endif	/* SERVER_H */