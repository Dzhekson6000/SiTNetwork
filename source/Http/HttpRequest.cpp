#include "Http/HttpRequest.h"

using namespace SiTNetwork;

HttpRequest::HttpRequest():
_method(METHOD::NONE)
{
}

HttpRequest::~HttpRequest()
{
}

std::string* HttpRequest::genRequest()
{
    _request.clear();
    _request.append(getMethodToString(_method));
    _request.append(" ").append(_path);
    _request.append(" ").append(_protocol).append("\r\n");
    
    for(auto header: _headers)
    {
        _request.append(header.first).append(": ").append(header.second).append("\r\n");
    }
    _request.append("\r\n\r\n");

    int isBegin = true;
    for(auto var: _vars)
    {
        if(!isBegin)_request.append("&");
        _request.append(var.first).append("=").append(var.second);
        isBegin = false;
    }
    
    return &_request;
}

void HttpRequest::parseRequest(const std::string &request)  throw(HttpParseError)
{
    _request.clear();
    _headers.clear();
    _vars.clear();
    
    std::string findStr("\r\n");
    std::string tmp;
    
    size_t prev = 0;
    size_t delta = findStr.length();
    
    size_t next = request.find(findStr);
    if (next == std::string::npos)
        throw HttpParseError("Not correct format HTTP");
    
    tmp = request.substr(0, next);
    parseMethodPathProtocol(tmp);
    prev = next+delta;

    while( (next=request.find(findStr,prev)) != std::string::npos)
    {
        tmp = request.substr(prev, next-prev);
        prev = next+delta;
        if(tmp == "")
        {
            break;
        }
        parseHeader(tmp);
    }
    
    parseBody(request.substr(prev));
}

void HttpRequest::parseMethodPathProtocol(const std::string &line)
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
    
    setPath(parsePath(tmp));
    
    //parse protocol
    tmp = line.substr(prev);
    setProtocol(tmp);
}

std::string HttpRequest::parsePath(const std::string &url)
{
    std::string path;
    std::string findStr("?");
    std::string tmp;
    size_t prev = 0;
    size_t delta = findStr.length();
    
    size_t next=url.find(findStr, prev);
    if (next == std::string::npos)
        return url;
    path = url.substr(prev, next-prev);
    prev = next+delta;
    
    tmp = url.substr(prev);
    parseVars(tmp);
    
    return path;
}


void HttpRequest::parseHeader(const std::string &line)
{
    std::string findStr(": ");
    std::string key;
    std::string value;
    
    size_t prev = 0;
    size_t delta = findStr.length();
    
    size_t next=line.find(findStr, prev);
    if (next == std::string::npos)
        throw HttpParseError("Not Found Header: Not correct format HTTP");
    
    key = line.substr(prev, next-prev);
    prev = next+delta;
    value = line.substr(prev);
    addHeader(key, value);
}

void HttpRequest::parseBody(const std::string &line)
{
    parseVars(line);
}

void HttpRequest::parseVars(const std::string& line)
{
    std::string findStr("&");
    std::string tmp;
    
    size_t next;
    size_t prev = 0;
    size_t delta = findStr.length();
    
    while( (next=line.find(findStr,prev)) != std::string::npos)
    {
        tmp = line.substr(prev, next-prev);
        prev = next+delta;
        parseVar(tmp);
    }
    
    tmp = line.substr(prev);
    if(tmp!="")
        parseVar(tmp);
    
}

void HttpRequest::parseVar(const std::string& line)
{
    std::string findStr("=");
    std::string key;
    std::string value;
    
    size_t prev = 0;
    size_t delta = findStr.length();
    
    size_t next=line.find(findStr, prev);
    if (next == std::string::npos)
        throw HttpParseError("Not Found Key: Not correct format HTTP");
    
    key = line.substr(prev, next-prev);
    prev = next+delta;
    value = line.substr(prev);
    addVar(key, value);
}


void HttpRequest::setMethod(METHOD method)
{
    _method = method;
}

void HttpRequest::setPath(std::string path)
{
    _path = path;
}

void HttpRequest::setProtocol(std::string protocol)
{
    _protocol = protocol;
}

void HttpRequest::addHeader(std::string key, std::string value)
{
    for(auto &header : _headers) {
        if (header.first == key) {
            header.second = value;
            return;
        }
    }
    _headers.push_back(std::make_pair(key, value));
}

void HttpRequest::addVar(std::string key, std::string value)
{
    for(auto &var : _vars) {
        if (var.first == key) {
            var.second = value;
            return;
        }
    }
    _vars.push_back(std::make_pair(key, value));
}


HttpRequest::METHOD HttpRequest::getMethod()
{
    return _method;
}

std::string HttpRequest::getPath()
{
    return _path;
}

std::string HttpRequest::getProtocol()
{
    return _protocol;
}


std::string HttpRequest::getHeader(std::string key) const
{
    for(auto &header : _headers)
    {
        if (header.first == key)
        {
            return header.second;
        }
    }
    return "";
}

std::string HttpRequest::getMethodToString(METHOD method) {
    switch(method)
    {
        case METHOD::GET:
            return std::string("GET");
        case METHOD::POST:
            return std::string("POST");
        case METHOD::NONE:
            return std::string("");
    }
}

HttpRequest::METHOD HttpRequest::getStringToMethod(const std::string &method)
{
    if(method == "GET")
    {
        return METHOD::GET;
    }
    else if(method == "POST")
    {
        return METHOD::POST;
    }
    else
    {
        return METHOD::NONE;
    }
}
