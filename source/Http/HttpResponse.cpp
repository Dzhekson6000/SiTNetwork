#include "Http/HttpResponse.h"
#include <stdio.h>
#include <stdlib.h>

using namespace SiTNetwork;

std::map<int, const char *> HttpResponse::_statuses =
{
    {100, "Continue"},
    {101, "Switching Protocols"},
    {200, "OK"},
    {201, "Created"},
    {202, "Accepted"},
    {203, "Non-Authoritative Information"},
    {204, "No Content"},
    {205, "Reset Content"},
    {206, "Partial Content"},
    {300, "Multiple Choices"},
    {301, "Moved Permanently"},
    {302, "Moved Temporarily"},
    {303, "See Other"},
    {304, "Not Modified"},
    {305, "Use Proxy"},
    {400, "Bad Request"},
    {401, "Unauthorized"},
    {402, "Payment Required"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {405, "Method Not Allowed"},
    {406, "Not Acceptable"},
    {407, "Proxy Authentication Required"},
    {408, "Request Time-out"},
    {409, "Conflict"},
    {410, "Gone"},
    {411, "Length Required"},
    {412, "Precondition Failed"},
    {413, "Request Entity Too Large"},
    {414, "Request-URI Too Large"},
    {415, "Unsupported Media Type"},
    {500, "Internal Server Error"},
    {501, "Not Implemented"},
    {502, "Bad Gateway"},
    {503, "Service Unavailable"},
    {504, "Gateway Time-out"},
    {505, "HTTP Version not supported"}
};

HttpResponse::HttpResponse():
_status(200)
{
}

HttpResponse::~HttpResponse()
{
}

std::string* HttpResponse::gen()
{
    char statusStr[6] = "";
    snprintf(statusStr, 6,"%d",_status);
    
    _http.clear();
    _http.append(_protocol);
    _http.append(" ").append(statusStr);
    _http.append(" ").append(getStatus(_status)).append("\r\n");
    
    for(auto header: _headers)
    {
        _http.append(header.first).append(": ").append(header.second).append("\r\n");
    }
    _http.append("\r\n\r\n");
    _http.append(_body);
    
    return &_http;
}

void HttpResponse::parseZeroLine(const std::string &line)
{
    std::string findStr(" ");
    std::string tmp;
    
    size_t prev = 0;
    size_t delta = findStr.length();
    
    //parse protocol
    size_t next=line.find(findStr, prev);
    if (next == std::string::npos)
        throw HttpParseError("Not Found string METHOD: Not correct format HTTP");
    tmp = line.substr(prev, next-prev);
    prev = next+delta;
    
    setProtocol(tmp);
    
    //parse status
    tmp = line.substr(prev);
    setStatus(std::stoi(tmp,nullptr));
}

void HttpResponse::parseBody(const std::string &line)
{
    setBody(line);
}

void HttpResponse::setStatus(unsigned int status)
{
    _status = status;
}

unsigned int HttpResponse::getStatus()
{
    return _status;
}

const char* HttpResponse::getStatus(int status)
{
    return _statuses[status];
}