#include "Http/HttpRequest.h"

using namespace SiTNetwork;

HttpRequest::HttpRequest()
{
}

HttpRequest::~HttpRequest()
{
}

std::string* HttpRequest::gen()
{
    _http.clear();
    _http.append(getMethodToString(_method));
    _http.append(" ").append(_path);
    _http.append(" ").append(_protocol).append("\r\n");
    
    for(auto header: _headers)
    {
        _http.append(header.first).append(": ").append(header.second).append("\r\n");
    }
    _http.append("\r\n\r\n");

    int isBegin = true;
    for(auto var: _vars)
    {
        if(!isBegin)_http.append("&");
        _http.append(var.first).append("=").append(var.second);
        isBegin = false;
    }
    
    return &_http;
}

void HttpRequest::parseZeroLine(const std::string &line)
{
    std::string findStr(" ");
    std::string tmp;
    
    size_t prev = 0;
    size_t delta = findStr.length();
    
    //parse METHOD
    size_t next=line.find(findStr, prev);
    if (next == std::string::npos)
        throw HttpParseError("Not Found string METHOD: Not correct format HTTP");
    tmp = line.substr(prev, next-prev);
    prev = next+delta;
    
    METHOD method = getStringToMethod(tmp);
    if(method == METHOD::NONE)
        throw HttpParseError("Not Found METHOD: Not correct format HTTP");
    setMethod(method);
    
    //parse URL
    next=line.find(findStr, prev);
    if (next == std::string::npos)
        throw HttpParseError("Not Found string METHOD: Not correct format HTTP");
    tmp = line.substr(prev, next-prev);
    prev = next+delta;
    
    tmp = parsePath(tmp);
    setPath(tmp);
    
    //parse protocol
    tmp = line.substr(prev);
    setProtocol(tmp);
}

void HttpRequest::parseBody(const std::string &line)
{
    parseVars(line);
}