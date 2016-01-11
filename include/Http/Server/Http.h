#ifndef HTTP_H
#define	HTTP_H

#include <string>
#include "Network/Server/ServerClient.h"

#define TRIM(str) while(isspace(*str))str++;

class Http: public ServerClient {
public:
    enum HTTP_STATUS
    {
        METHOD = 0,
        FILENAME = 1,
        VERSION = 2
    };
    
    enum LINE_STATUS
    {
        LINE_BAD = 0,
        LINE_OK = 1,
        LINE_NONE = 2,
        LINE_END=3
    };
    
    Http(int clientSocket);
    virtual ~Http();
    
    static const char * getType(std::string *filename);
    
protected:
    char _method[255];
    char _filename[255];
    char _version[255];
    
    LINE_STATUS         parse_line(int socket,char *buffer);
    void                parse_request(int socket,char *buffer);
    void                deal_request(int socket,char *method,char *filename,char *version);
    static const char * getType(char *filename);
    
    static void headers_200(int socket,const char *type,unsigned int size);
    static void headers_404(int socket);
    
    virtual void thread();
};

#endif	/* HTTP_H */

