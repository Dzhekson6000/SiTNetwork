#include "HttpRequest.h"

using namespace doxyCraft;

HttpRequest::HttpRequest()
{
}

HttpRequest::~HttpRequest()
{
}

String* HttpRequest::gen()
{
    _http.clear();
    _startingLine.begin = _http.size();
    _http.append(getMethodAtString(_method).c_str());
    _http.append(" ").append(_path.c_str());
    _http.append(" ").append(getProtocolAtString(_protocol).c_str()).append("\r\n");
    _startingLine.begin = _http.size();

    String body;
    bool isBegin = true;
    for(auto var: _vars)
    {
	if(!isBegin)body.append("&");
	body.append(var.first.c_str()).append("=").append(var.second.c_str());
	isBegin = false;
    }
    
    if(!_isKeepAlive)
	addHeader("Connection", "close");
    else
	addHeader("Connection", "Keep-Alive");
    if(!_isChunked)addHeader("Content-Length", String() + body.size());

    _head.begin = _http.size();
    for(auto header: _headers)
    {
	_http.append(header.first.c_str()).append(": ").append(header.second.c_str()).append("\r\n");
    }
    _head.end = _http.size();


    _http.append("\r\n");
    _body.begin = _http.size();
    _http.append(body.c_str());
    _body.end = _http.size();

    return &_http;
}

bool HttpRequest::parseStartingLine(const String &line)
{
    String findStr(" ");
    String tmp;

    size_t prev = 0;
    size_t delta = findStr.length();

    //parse METHOD
    size_t next=line.find(findStr.c_str(), prev);
    if (next == String::npos)
	return false;
    tmp = line.substr(prev, next-prev);
    prev = next+delta;

    METHOD method = getMethodFromString(tmp);
    setMethod(method);

    //parse URL
    next=line.find(findStr.c_str(), prev);
    if (next == String::npos)
	return false;
    tmp = line.substr(prev, next-prev);
    prev = next+delta;

    tmp = parsePath(tmp);
    setPath(tmp);

    //parse protocol
    tmp = line.substr(prev);
    setProtocol(getProtocolFromString(tmp));
    return Http::parseStartingLine(line);
}

bool HttpRequest::parseBody(const String &line)
{
    parseVars(line);
    return Http::parseBody(line);
}