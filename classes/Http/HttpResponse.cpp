#include "HttpResponse.h"

using namespace doxyCraft;

std::map<int, const char *> HttpResponse::_statuses ={
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

String* HttpResponse::gen()
{
    char statusStr[6] = "";
#ifdef WINDOWS
    sprintf_s(statusStr, 6, "%d", _status);
#else
    snprintf(statusStr, 6, "%d", _status);
#endif
    
    _http.clear();
    _startingLine.begin = _http.size();
    _http.append(getProtocolAtString(_protocol).c_str());
    _http.append(" ").append(statusStr);
    _http.append(" ").append(getStatus(_status)).append("\r\n");
    _startingLine.end = _http.size();
    
	if (!isKeepAlive())
	{
		addHeader("Connection", "close");
	}
	else
	{
		addHeader("Connection", "keep-alive");
	}
	if (!_isChunked && getHeader("Content-Length").empty())
	{
		addHeader("Content-Length", String() + _bodyResponse.size());
	}
    
    _head.begin = _http.size();
    for(auto header: _headers)
    {
		_http.append(header.first.c_str()).append(": ").append(header.second.c_str()).append("\r\n");
    }

    _head.end = _http.size();
    _http.append("\r\n");

    _body.begin = _http.size();
    _http.append(_bodyResponse.c_str());
    _body.end = _http.size();

    return &_http;
}

bool HttpResponse::parseStartingLine(const String &line)
{
    String findStr(" ");
    String tmp;

    size_t prev = 0;
    size_t delta = findStr.length();

    //parse protocol
    size_t next=line.find(findStr.c_str(), prev);
	if (next == String::npos)
	{
		return false;
	}
    tmp = line.substr(prev, next-prev);
    prev = next+delta;

    setProtocol(getProtocolFromString(tmp));

    //parse status
    tmp = line.substr(prev);
    setStatus(std::stoi(tmp.c_str()));
    return Http::parseStartingLine(line);
}

bool HttpResponse::parseBody(const String &line)
{
    setBody(line);
    return Http::parseBody(line);
}

void HttpResponse::setStatus(unsigned int status)
{
    _status = status;
}

void HttpResponse::setBody(const String& body)
{
    _bodyResponse = body;
}

unsigned int HttpResponse::getStatus()
{
    return _status;
}

const char* HttpResponse::getStatus(int status)
{
    return _statuses[status];
}