#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H
#include "Http/Http.h"
#include <map>

namespace SiTNetwork
{
    class HttpResponse:public Http {
    public:
        HttpResponse();
        virtual ~HttpResponse();

        std::string* gen();
        
        void setStatus(unsigned int status);
        
        unsigned int getStatus();
        static const char* getStatus(int status);
    protected:
        unsigned int _status;
        static std::map<int, const char *> _statuses;
        
        void parseZeroLine(const std::string &line);
        void parseBody(const std::string &line);
    };
}

#endif /* HTTPRESPONSE_H */

