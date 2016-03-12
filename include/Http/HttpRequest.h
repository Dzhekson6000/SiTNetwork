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
	bool parseBody(const std::string &line);
    protected:
        bool parseStartingLine(const std::string &line);
    };
}

#endif /* HTTPREQUEST_H */

