#ifndef CLIENT_H
#define CLIENT_H
#include "Network/Socket.h"

namespace SiTNetwork
{
    class ServerClient
    {
    public:
	ServerClient(Socket* socketClient);
	virtual		~ServerClient();
    protected:
	Socket*		_socketClient;

	virtual void	thread();
    } ;
}
#endif /* CLIENT_H */

