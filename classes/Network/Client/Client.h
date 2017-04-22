#ifndef CLIENT_H
#define CLIENT_H

#include "Shared/Types/String/String.h"

namespace doxyCraft
{
    class Client
    {
    public:
	Client();
	Client(String host, int port);
	virtual	~Client();
    private:
    } ;
}

#endif /* CLIENT_H */

