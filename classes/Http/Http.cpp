#include "Http.h"
#include <sstream>

using namespace doxyCraft;

const char* strCRLF = "\r\n";

const char* strTwoCRLF = "\r\n\r\n";

const char* strLF = "\n";

const char* Http::PROTOCOL_STRING[] = {"HTTP/0.9", "HTTP/1.0", "HTTP/1.1", "HTTP/2"};

const char* Http::METHOD_STRING[] = {"OPTIONS", "GET", "HEAD", "POST", "PUT", "DELETE", "TRACE", "CONNECT"};

Http::Http():
		_method(METHOD::GET),
		_protocol(PROTOCOL::HTTP11),
		_path(),
		_http(),
		_parsePosition(0),
		_leftLoadBody(0),
		_isKnowSize(false),
		_isChunked(false),
		_isDateSize(true),
		_isEndTransfer(false),
		_parseStatus(PARSE_STATUS::PARSE_STARTLINE),
		_isKeepAlive(false)
{
	_startingLine = {0, 0};
	_head         = {0, 0};
	_body         = {0, 0};
}

Http::~Http()
{
}

void Http::clear()
{
	_method   = METHOD::GET;
	_protocol = PROTOCOL::HTTP11;
	_path.clear();
	_headers.clear();
	_vars.clear();
	
	_http.clear();
	_startingLine.begin = 0;
	_startingLine.end   = 0;
	_head.begin         = 0;
	_head.end           = 0;
	_body.begin         = 0;
	_body.end           = 0;
	
	_parsePosition = 0;
	_leftLoadBody  = 0;
	_isKnowSize    = false;
	_isChunked     = false;
	_isDateSize    = true;
	_isEndTransfer = false;
	
	_tempChunked.clear();
	
	_parseStatus = PARSE_STATUS::PARSE_STARTLINE;
	_isKeepAlive = false;
}

bool Http::isKnowSize()
{
	return _isKnowSize;
}

void Http::endTransfer()
{
	_isEndTransfer = true;
}

String* Http::gen()
{
	return &_http;
}

bool Http::parse()
{
	size_t next, delta;
	
	while( _parseStatus != PARSE_STATUS::PARSE_END )
	{
		if( _http.empty())
		{
			return false;
		}
		switch( _parseStatus )
		{
			case PARSE_STATUS::PARSE_STARTLINE:
				next = findNewLine(_http, 0, delta);
				if( next == String::npos )
				{
					return true;
				}
				if( !parseStartingLine(_http.substr(0, next)))
				{
					return false;
				}
				_startingLine.begin = _parsePosition;
				_parsePosition = next + delta;
				_startingLine.end = _parsePosition;
				break;
			case PARSE_STATUS::PARSE_HEAD:
				next = _http.find(strTwoCRLF, _parsePosition);
				if( next == String::npos )
				{
					return true;
				}
				if( !parseHead(_http.substr(_parsePosition, next - _parsePosition)))
				{
					return false;
				}
				_head.begin = _parsePosition;
				_parsePosition = next + (unsigned)strlen(strTwoCRLF);
				_head.end = _parsePosition;
				
				if( _isChunked && _http.size() - _parsePosition != 0 )
				{
					String tmp = _http.substr(_parsePosition);
					_http = _http.substr(0, _parsePosition);
					if( !parseNewChunked(tmp))
					{
						_http.append(tmp.c_str());
						return false;
					}
				}
				else if( _isChunked )
				{
					return true;
				}
				else if( !_isChunked )
				{
					_leftLoadBody -= _http.size() - _parsePosition;
				}
				break;
			case PARSE_STATUS::PARSE_BODY:
				if( !_isEndTransfer && (!_isKnowSize || _leftLoadBody != 0))
				{
					return true;
				}
				if( !parseBody(_http.substr(_parsePosition)))
				{
					return false;
				}
				_body.begin = _parsePosition;
				_parsePosition = _http.size();
				_body.end = _parsePosition;
				break;
			case PARSE_STATUS::PARSE_END:
				break;
		}
	}
	return true;
}

bool Http::parse(const String &request)
{
	clear();
	_http = request;
	return parse();
}

bool Http::parseNewDate(const String &date)
{
	if( _isChunked )
	{
		if( !parseNewChunked(date))
		{
			return false;
		}
	}
	else
	{
		if( _parseStatus == PARSE_STATUS::PARSE_BODY )
		{
			_leftLoadBody -= date.size();
		}
		_http.append(date.c_str());
		if( !parse())
		{
			return false;
		}
	}
	return true;
}

bool Http::parseNewChunked(const String &date)
{
	String chunk;
	if( !_tempChunked.empty())
	{
		chunk.append(_tempChunked.c_str());
		_tempChunked.clear();
	}
	chunk.append(date.c_str());
	
	size_t      next, delta, position = 0;
	String tmp;
	
	while( position < date.size())
	{
		next = findNewLine(chunk, position, delta);
		if( next == String::npos )
		{
			if( _isDateSize )
			{
				_tempChunked.append(chunk.substr(position).c_str());
			}
			else
			{
				_http.append(chunk.substr(position).c_str());
			}
			return true;
		}
		
		tmp      = chunk.substr(position, next - position);
		position = next + delta;
		
		if( _isDateSize )
		{
			unsigned int size = hexToDec(tmp);
			if( size == 0 )
			{
				endTransfer();
			}
			_leftLoadBody += size;
			_isDateSize       = false;
		}
		else
		{
			_http.append(tmp.c_str());
			_leftLoadBody -= tmp.size();
			_isDateSize = true;
		}
	}
	
	return parse();
}

bool Http::parseStartingLine(const String &line)
{
	_parseStatus = PARSE_STATUS::PARSE_HEAD;
	return true;
}

bool Http::parseHead(const String &head)
{
	size_t delta, next, prev = 0;
	while((next  = findNewLine(head, prev, delta)) != String::npos && prev != next )
	{
		if( !parseHeader(head.substr(prev, next - prev)))
		{
			return false;
		}
		prev = next + delta;
	}
	_parseStatus = PARSE_STATUS::PARSE_BODY;
	
	String keepAlive = getHeader("Connection");
	if( keepAlive.empty() || keepAlive == "keep-alive" )
	{
		_isKeepAlive = true;
	}
	
	String transferEncoding = getHeader("Transfer-Encoding");
	if( !transferEncoding.empty() && transferEncoding == "chunked" )
	{
		_isChunked = true;
		return true;
	}
	
	String contentLength = getHeader("Content-Length");
	if( !contentLength.empty())
	{
		_isKnowSize   = true;
		_leftLoadBody = atoi(contentLength.c_str());
	}
	
	return true;
}

bool Http::parseBody(const String &body)
{
	_parseStatus = PARSE_STATUS::PARSE_END;
	return true;
}

size_t Http::findNewLine(const String &request, const size_t &begin, size_t &delta)
{
	size_t next = request.find(strCRLF, begin);
	if( next != String::npos )
	{
		delta = (unsigned)strlen(strCRLF);
		return next;
	}
	next        = request.find(strLF, begin);
	delta       = (unsigned)strlen(strLF);
	return next;
}

String Http::parsePath(const String &url)
{
	String path;
	String findStr("?");
	String tmp;
	size_t      prev  = 0;
	size_t      delta = findStr.length();
	
	size_t next = url.find(findStr.c_str(), prev);
	if( next == String::npos )
	{
		return url;
	}
	path = url.substr(prev, next - prev);
	prev = next + delta;
	
	tmp = url.substr(prev);
	parseVars(tmp);
	
	return path;
}

bool Http::parseHeader(const String &line)
{
	String findStr(": ");
	String key;
	String value;
	
	size_t prev  = 0;
	size_t delta = findStr.length();
	
	size_t next = line.find(findStr.c_str(), prev);
	if( next == String::npos )
	{
		return false;
	}
	
	key   = line.substr(prev, next - prev);
	prev  = next + delta;
	value = line.substr(prev);
	addHeader(key, value);
	return true;
}

void Http::parseVars(const String &line)
{
	String findStr("&");
	String tmp;
	
	size_t next;
	size_t prev  = 0;
	size_t delta = findStr.length();
	
	while((next = line.find(findStr.c_str(), prev)) != String::npos )
	{
		tmp  = line.substr(prev, next - prev);
		prev = next + delta;
		parseVar(tmp);
	}
	
	tmp = line.substr(prev);
	if( tmp != "" )
	{
		parseVar(tmp);
	}
	
}

void Http::parseVar(const String &line)
{
	String findStr("=");
	String key;
	String value;
	
	size_t prev  = 0;
	size_t delta = findStr.length();
	
	size_t next = line.find(findStr.c_str(), prev);
	if( next == String::npos )
	{
		return;
	}
	
	key   = line.substr(prev, next - prev);
	prev  = next + delta;
	value = line.substr(prev);
	addVar(key, value);
}

void Http::setMethod(METHOD method)
{
	_method = method;
}

void Http::setPath(const String &path)
{
	_path = path;
}

void Http::setProtocol(PROTOCOL protocol)
{
	_protocol = protocol;
}

void Http::addHeader(const String &key, const String &value)
{
	_headers.insert(std::make_pair(key, value));
}

void Http::setHeaders(hashMap &headers)
{
	_headers = headers;
}

void Http::addVar(const String &key, const String &value)
{
	_vars.insert(std::make_pair(key, value));
}

const String* Http::getHttp() const
{
	return &_http;
}

const String Http::getStartingLine() const
{
	return _http.substr(_startingLine.begin, _startingLine.end - _startingLine.begin);
}

const String Http::getHead() const
{
	return _http.substr(_head.begin, _head.end - _head.begin);
}

const String Http::getBody() const
{
	return _http.substr(_body.begin, _body.end - _body.begin);
}

Http::METHOD Http::getMethod()
{
	return _method;
}

String Http::getPath() const
{
	return _path;
}

Http::PROTOCOL Http::getProtocol() const
{
	return _protocol;
}

String Http::getHeader(const String &key) const
{
	auto it = _headers.find(key);
	return it != _headers.end() ? it->second : "";
}

String Http::getVar(const String &key) const
{
	auto it = _vars.find(key);
	return it != _vars.end() ? it->second : "";
}

hashMap Http::getHeaders() const
{
	return _headers;
}

hashMap Http::getVars() const
{
	return _vars;
}

Http::PARSE_STATUS Http::getParseStatus()
{
	return _parseStatus;
}

String Http::getMethodAtString(METHOD method)
{
	return METHOD_STRING[static_cast<unsigned> (method)];
}

Http::METHOD Http::getMethodFromString(const String &method)
{
	int       i = 0;
	for( auto methodString : METHOD_STRING )
	{
		if( methodString == method )
		{
			break;
		}
		i++;
	}
	return static_cast<METHOD> (i);
}

String Http::getProtocolAtString(PROTOCOL protocol)
{
	return PROTOCOL_STRING[static_cast<unsigned> (protocol)];
}

Http::PROTOCOL Http::getProtocolFromString(const String &protocol)
{
	int       i = 0;
	for( auto protocolString : PROTOCOL_STRING )
	{
		if( protocolString == protocol )
		{
			break;
		}
		i++;
	}
	return static_cast<PROTOCOL> (i);
}

unsigned int Http::hexToDec(const String &hex)
{
	unsigned int      dec;
	std::stringstream ss;
	
	ss<<std::hex<<hex.c_str();
	ss>>dec;
	
	return dec;
}

bool Http::isKeepAlive() const
{
	return _isKeepAlive;
}

void Http::setKeepAlive(bool isKeepAlive)
{
	_isKeepAlive = isKeepAlive;
}

