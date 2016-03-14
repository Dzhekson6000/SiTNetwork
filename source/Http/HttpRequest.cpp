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
    _startingLine.begin = _http.size();
    _http.append(getMethodAtString(_method));
    _http.append(" ").append(_path);
    _http.append(" ").append(getProtocolAtString(_protocol)).append("\r\n");
    _startingLine.begin = _http.size();

    std::string body;
    bool isBegin = true;
    for(auto var: _vars)
    {
	if(!isBegin)body.append("&");
	body.append(var.first).append("=").append(var.second);
	isBegin = false;
    }
    
    if(!_isKeepAlive)
	addHeader("Connection", "close");
    else
	addHeader("Connection", "Keep-Alive");
    if(!_isChunked)addHeader("Content-Length", std::to_string(body.size()));

    _head.begin = _http.size();
    for(auto header: _headers)
    {
	_http.append(header.first).append(": ").append(header.second).append("\r\n");
    }
    _head.end = _http.size();


    _http.append("\r\n");
    _body.begin = _http.size();
    _http.append(body);
    _body.end = _http.size();

    return &_http;
}

bool HttpRequest::parseStartingLine(const std::string &line)
{
    std::string findStr(" ");
    std::string tmp;

    size_t prev = 0;
    size_t delta = findStr.length();

    //parse METHOD
    size_t next=line.find(findStr, prev);
    if (next == std::string::npos)
	return false;
    tmp = line.substr(prev, next-prev);
    prev = next+delta;

    METHOD method = getMethodFromString(tmp);
    setMethod(method);

    //parse URL
    next=line.find(findStr, prev);
    if (next == std::string::npos)
	return false;
    tmp = line.substr(prev, next-prev);
    prev = next+delta;

    tmp = parsePath(tmp);
    setPath(tmp);

    //parse protocol
    tmp = line.substr(prev);
    setProtocol(getProtocolFromString(tmp));
    return true;
}

bool HttpRequest::parseBody(const std::string &line)
{
    parseVars(line);
    return true;
}