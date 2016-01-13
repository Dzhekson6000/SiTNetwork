#include <stdlib.h> 
#include "Network/Server/Server.h"
#include <thread>

class LogDeb
{
public:
    LogDeb()
    {
        
    };
    
    void Log(std::string message)
    {
        perror(message.c_str());
    }
    
};

using namespace SiTNetwork;

int main(int argc , char *argv[])
{
    LogDeb logDeb;
    
    Server server(8000);
    server.setLogFunc(NET_CALLBACK_1(LogDeb::Log, logDeb));
    std::thread(&Server::run, &server).detach();
    
    char cmd[20];
    while (true) {
        scanf("%20s",cmd);
        if(strcmp(cmd,"stop") == 0)
        {
            server.stop();
            return 0;
        }
    }
    
    return 0;
}