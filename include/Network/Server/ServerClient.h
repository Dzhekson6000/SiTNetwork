#ifndef CLIENT_H
#define	CLIENT_H

class ServerClient {
public:    
    ServerClient(int clientSocket);
    virtual ~ServerClient();
    
    static void* run(void* thisPtr);
protected:
    int _clientSocket;
    
    virtual void thread();
};

#endif	/* CLIENT_H */

