#ifndef CLIENT_H
#define CLIENT_H

#include <string>

namespace SiTNetwork
{
    class Client {
    public:
        Client();
        Client(std::string host, int port);
        virtual ~Client();
    private:
    };
}

#endif /* CLIENT_H */

