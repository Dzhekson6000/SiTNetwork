#include "Http/Http.h"
#include <cstring>

using namespace SiTNetwork;

const char* strCRLF = "\r\n";
const char* strLF = "\n";

const char* Http::PROTOCOL_STRING[] = {"HTTP/0.9","HTTP/1.0","HTTP/1.1","HTTP/2"};
const char* Http::METHOD_STRING[] = {"OPTIONS","GET","HEAD","POST","PUT","DELETE","TRACE","CONNECT"};

Http::Http():
_method(METHOD::GET)
{
}

Http::~Http()
{
}

std::string* Http::gen()
{
    return &_http;
}

bool Http::parse(const std::string &request)
{
    _http.clear();
    _headers.clear();
    _vars.clear();
    
    std::string tmp;
    
    size_t prev = 0;
    size_t delta;
    
    size_t next = findNewLine(request, 0, delta);
    if (next == std::string::npos)
        return false;
    
    tmp = request.substr(0, next);
    if(!parseStartingLine(tmp))return false;
    prev = next+delta;

    while( (next=findNewLine(request, prev, delta)) != std::string::npos)
    {
        tmp = request.substr(prev, next-prev);
        prev = next+delta;
        if(tmp == "")
        {
            break;
        }
        if(!parseHeader(tmp))return false;
    }
    
    parseBody(request.substr(prev));
    return true;
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

bool Http::parseStartingLine(const std::string &line)
{
    return true;
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


bool Http::parseHeader(const std::string &line)
{
    std::string findStr(": ");
    std::string key;
    std::string value;
    
    size_t prev = 0;
    size_t delta = findStr.length();
    
    size_t next=line.find(findStr, prev);
    if (next == std::string::npos)
        return false;
    
    key = line.substr(prev, next-prev);
    prev = next+delta;
    value = line.substr(prev);
    addHeader(key, value);
    return true;
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
        return;
    
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

void Http::setProtocol(PROTOCOL protocol)
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

void Http::setHeaders(std::vector<std::pair<std::string, std::string> >& headers)
{
    _headers = headers;
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

const std::string* Http::getHttp() const
{
    return &_http;
}

const std::string* Http::getBody() const
{
    return &_body;
}

Http::METHOD Http::getMethod()
{
    return _method;
}

std::string Http::getPath() const
{
    return _path;
}

Http::PROTOCOL Http::getProtocol() const
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

std::string Http::getVar(std::string key) const
{
    for(auto &var : _vars)
    {
        if (var.first == key)
        {
            return var.second;
        }
    }
    return "";
}

std::vector<std::pair<std::string, std::string>> Http::getHeaders() const
{
    return _headers;
}

std::vector<std::pair<std::string, std::string> > Http::getVars() const
{
    return _vars;
}

std::string Http::getMethodAtString(METHOD method)
{
    return METHOD_STRING[static_cast<unsigned>(method)];
}

Http::METHOD Http::getMethodFromString(const std::string &method)
{
    int i = 0;
    for(auto methodString: METHOD_STRING)
    {
	if(methodString==method)break;
	i++;
    }
    return static_cast<METHOD>(i);
}

std::string Http::getProtocolAtString(PROTOCOL protocol)
{
    return PROTOCOL_STRING[static_cast<unsigned>(protocol)];
}

Http::PROTOCOL Http::getProtocolFromString(const std::string& protocol)
{
    int i = 0;
    for(auto protocolString: PROTOCOL_STRING)
    {
	if(protocolString==protocol)break;
	i++;
    }
    return static_cast<PROTOCOL>(i);
}
