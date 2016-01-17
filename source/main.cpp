#include <stdlib.h> 
#include "Network/Server/Server.h"
#include <thread>
#include "Http/HttpRequest.h"
#include "Http/HttpResponse.h"

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
    std::string str = "HTTP/1.1 200 OK"
            "Host: site.com\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n"
            "Connection: close\r\n"
            "Content-Length: 21\r\n"
            "\r\n"
            "<h1>Test page...</h1>";
    
    
    HttpResponse hr = HttpResponse();
    hr.parse(str);
    
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