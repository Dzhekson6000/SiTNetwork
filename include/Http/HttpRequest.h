#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <vector>
#include "HttpParseError.h"

namespace SiTNetwork
{
    class HttpRequest
    {
    public:
        enum METHOD
        {
            OPTIONS,
            GET,
            HEAD,
            POST,
            PUT,
            DELETE,
            TRACE,
            CONNECT,
            NONE
        };
        
        HttpRequest();
        virtual ~HttpRequest();

        std::string* genRequest();
        void parseRequest(const std::string &request) throw(HttpParseError);
        
        void setMethod(METHOD method);
        void setPath(std::string path);
        void setProtocol(std::string protocol);
        void addHeader(std::string key, std::string value);
        void addVar(std::string key, std::string value);
        
        METHOD getMethod();
        std::string getPath();
        std::string getProtocol();
        std::string getHeader(std::string key) const;
        std::string getMethodToString(METHOD method);
        METHOD getStringToMethod(const std::string &method);
    protected:
        METHOD _method;
        std::string _path;
        std::string _protocol;
        std::vector< std::pair<std::string, std::string>> _headers;
        std::vector< std::pair<std::string, std::string>> _vars;
        
        std::string _request;
        
        void parseMethodPathProtocol(const std::string &line);
        std::string parsePath(const std::string &url);
        void parseHeader(const std::string &line);
        void parseBody(const std::string &line);
        void parseVars(const std::string &line);
        void parseVar(const std::string &line);
    };
}

#endif /* HTTPREQUEST_H */

