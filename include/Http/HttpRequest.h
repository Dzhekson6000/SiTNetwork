#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "Http/Http.h"

namespace SiTNetwork
{
    class HttpRequest:public Http {
    public:
        HttpRequest();
        virtual ~HttpRequest();
        
        std::string* gen();
    protected:
        void parseZeroLine(const std::string &line);
        void parseBody(const std::string &line);
    };
}

#endif /* HTTPREQUEST_H */

