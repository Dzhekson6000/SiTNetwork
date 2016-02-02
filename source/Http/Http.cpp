#include "Http/Http.h"

using namespace SiTNetwork;

const char* strCRLF = "\r\n";
const char* strLF = "\n";

Http::Http():
_method(METHOD::NONE)
{
}

Http::~Http()
{
}

std::string* Http::gen()
{
    return &_http;
}

void Http::parse(const std::string &request)  throw(HttpParseError)
{
    _http.clear();
    _headers.clear();
    _vars.clear();
    
    std::string tmp;
    
    size_t prev = 0;
    size_t delta;
    
    size_t next = findNewLine(request, 0, delta);
    if (next == std::string::npos)
        throw HttpParseError("Not correct format HTTP");
    
    tmp = request.substr(0, next);
    parseZeroLine(tmp);
    prev = next+delta;

    while( (next=findNewLine(request, prev, delta)) != std::string::npos)
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

size_t Http::findNewLine(const std::string& request, const size_t& begin, size_t& delta)
{
    size_t next = request.find(strCRLF, begin);
    if (next != std::string::npos)
    {
        delta = (unsigned)strlen(strCRLF);
        return next;
    }
    next = request.find(strLF, begin);
    delta = (unsigned)strlen(strLF);
    return next;
}

void Http::parseZeroLine(const std::string &line)
{
}

std::string Http::parsePath(const std::string &url)
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


void Http::parseHeader(const std::string &line)
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

void Http::parseBody(const std::string &line)
{
}

void Http::parseVars(const std::string& line)
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

void Http::parseVar(const std::string& line)
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


void Http::setMethod(METHOD method)
{
    _method = method;
}

void Http::setPath(const std::string &path)
{
    _path = path;
}

void Http::setProtocol(const std::string &protocol)
{
    _protocol = protocol;
}

void Http::setBody(const std::string &body)
{
    _body = body;
}


void Http::addHeader(std::string key, std::string value)
{
    for(auto &header : _headers) {
        if (header.first == key) {
            header.second = value;
            return;
        }
    }
    _headers.push_back(std::make_pair(key, value));
}

void Http::addVar(std::string key, std::string value)
{
    for(auto &var : _vars) {
        if (var.first == key) {
            var.second = value;
            return;
        }
    }
    _vars.push_back(std::make_pair(key, value));
}


Http::METHOD Http::getMethod()
{
    return _method;
}

std::string Http::getPath() const
{
    return _path;
}

std::string Http::getProtocol() const
{
    return _protocol;
}


std::string Http::getHeader(std::string key) const
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

std::string Http::getBody() const
{
    return _body;
}


std::string Http::getMethodToString(METHOD method) {
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

Http::METHOD Http::getStringToMethod(const std::string &method)
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

const std::string* Http::getHttp() const
{
    return &_http;
}
