#ifndef CLIENT_H
#define CLIENT_H
#include "../Socket.h"

namespace doxyCraft
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

