#include <stdlib.h> 
#include "Network/Server/Server.h"

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
    
    void LogError(std::string message)
    {
        perror(message.c_str());
    }
    
    //perror(message.c_str());
    
};


int main(int argc , char *argv[])
{
    //system("cd /home/dzhekson6000/workspace/SiTEngineServerCore/test/ && "
    //        "git clone https://github.com/Dzhekson6000/SiTEngineSite");
    
    /*
    pthread_t tid;
    Server server(8000);
    pthread_create(&tid, NULL, Server::run, &server);
    
    char cmd[20];
    while (true) {
        scanf("%20s",cmd);
        if(strcmp(cmd,"stop") == 0)
        {
            server.stop();
            return 0;
        }
    }*/
    
    LogDeb logDeb;
    
    pthread_t tid;
    Server server(8000);
    server.setLogFunc(NET_CALLBACK_1(LogDeb::Log, logDeb));
    server.setLogErrorFunc(NET_CALLBACK_1(LogDeb::LogError, logDeb));
    pthread_create(&tid, NULL, Server::run, &server);
    
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