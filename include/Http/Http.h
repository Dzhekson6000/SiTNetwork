#ifndef HTTP_H
#define HTTP_H

#include <string>
#include <vector>
#include "HttpParseError.h"

namespace SiTNetwork
{
    class Http
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
        
        Http();
        virtual ~Http();

        virtual std::string* gen();
        void parse(const std::string &request) throw(HttpParseError);
        
        void setMethod(METHOD method);
        void setPath(const std::string &path);
        void setProtocol(const std::string &protocol);
        void setBody(const std::string &body);
        void addHeader(std::string key, std::string value);
        void addVar(std::string key, std::string value);
        
        METHOD getMethod();
        std::string getPath() const;
        std::string getProtocol() const;
        std::string getBody() const;
        std::string getHeader(std::string key) const;
        std::string getMethodToString(METHOD method);
        METHOD getStringToMethod(const std::string &method);
        const std::string* getHttp() const;
    protected:
        METHOD _method;
        std::string _path;
        std::string _protocol;
        std::vector< std::pair<std::string, std::string>> _headers;
        std::vector< std::pair<std::string, std::string>> _vars;
        
        std::string _http;
        std::string _body;
        
        size_t findNewLine(const std::string &request, const size_t &begin, size_t& delta);
        
        virtual void parseZeroLine(const std::string &line);
        std::string parsePath(const std::string &url);
        void parseHeader(const std::string &line);
        virtual void parseBody(const std::string &line);
        void parseVars(const std::string &line);
        void parseVar(const std::string &line);
    };
}

#endif /* HTTP_H */

