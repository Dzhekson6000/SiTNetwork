#ifndef CLIENT_H
#define	CLIENT_H
#include <pthread.h>

class ServerClient {
public:    
    ServerClient(int clientSocket);
    virtual ~ServerClient();
    
    static void* run(void* thisPtr);
protected:
    pthread_t _tid;
    int _clientSocket;
    
    virtual void thread();
};

#endif	/* CLIENT_H */

