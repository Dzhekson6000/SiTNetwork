#ifndef SERVER_H
#define SERVER_H

#include "Shared/Types/String/String.h"
#include <functional>
#include "ServerClient.h"

#define NET_CALLBACK_1(func, _Object) std::bind(&func,_Object, std::placeholders::_1)
#define NET_CALLBACK_2(func, _Object) std::bind(&func,_Object, std::placeholders::_1, std::placeholders::_2)
#define NET_CALLBACK_3(func, _Object) std::bind(&func,_Object, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

namespace doxyCraft
{
    typedef std::function<void(String)>		OnLogFunc;
    typedef std::function<void(Socket* clientSocket)>	OnNewClientFunc;
    class Server
    {
    public:
	Server();
	Server(int port);
	virtual		~Server();
	void		start();
	void		stop();
	void		setLogFunc(OnLogFunc onLogFunc);

	void		setNewClientFunc(OnNewClientFunc onNewClientFunc);
	void		setUseSSL(bool isUseSSL);
	bool		getUseSSL();


	static void*	run(void* thisPtr);
    private:
	bool		_isUseSSL;
	int		_port;
	bool		_isClose;
	void		log(String message);
	void		newClient(Socket* socketClient);

	OnLogFunc	_onLogFunc;
	OnNewClientFunc	_onNewClientFunc;
    } ;
}

#endif /* SERVER_H */