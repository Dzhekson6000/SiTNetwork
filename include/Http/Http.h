#ifndef HTTP_H
#define HTTP_H

#include <string>
#include <vector>

namespace SiTNetwork
{
    class Http
    {
    public:
	typedef std::vector<std::pair<std::string, std::string>> vectorVar;
	struct UnitDate
	{
	    unsigned int begin;
	    unsigned int end;
	} ;
	enum class PROTOCOL : unsigned int
	{
	    HTTP09 = 0,
	    HTTP10 = 1,
	    HTTP11 = 2,
	    HTTP2 = 3,
	    COUNT = 4
	} ;
	enum class METHOD : unsigned int
	{
	    OPTIONS = 0,
	    GET = 1,
	    HEAD = 2,
	    POST = 3,
	    PUT = 4,
	    DELETE = 5,
	    TRACE = 6,
	    CONNECT = 7,
	    COUNT = 8
	} ;
	enum class PARSE_STATUS
	{
	    PARSE_STARTLINE,
	    PARSE_HEAD,
	    PARSE_BODY,
	    PARSE_END
	} ;

	static const char*	PROTOCOL_STRING[];
	static const char*	METHOD_STRING[];

	Http();
	virtual			~Http();

	void			clear();
	void			endTransfer();

	virtual std::string*	gen();
	bool			parse();
	bool			parse(const std::string &request);

	bool			parseNewDate(const std::string &date);
	bool			parseNewChunked(const std::string &date);

	virtual bool		parseStartingLine(const std::string &line);
	bool			parseHead(const std::string &head);
	virtual bool		parseBody(const std::string& body);

	void			setMethod(METHOD method);
	void			setPath(const std::string &path);
	void			setProtocol(PROTOCOL protocol);
	void			addHeader(std::string key, std::string value);
	void			setHeaders(vectorVar& headers);
	void			addVar(std::string key, std::string value);

	const std::string*	getHttp() const;
	const std::string	getStartingLine() const;
	const std::string	getHead() const;
	const std::string	getBody() const;

	METHOD			getMethod();
	std::string		getPath() const;
	PROTOCOL		getProtocol() const;

	std::string		getHeader(std::string key) const;
	std::string		getVar(std::string key) const;

	vectorVar		getHeaders() const;
	vectorVar		getVars() const;

	PARSE_STATUS		getParseStatus();

	std::string		getMethodAtString(METHOD method);
	METHOD			getMethodFromString(const std::string& method);

	std::string		getProtocolAtString(PROTOCOL protocol);
	PROTOCOL		getProtocolFromString(const std::string& protocol);
	
	bool			isKeepAlive();
	void			setKeepAlive(bool isKeepAlive);
    protected:
	METHOD                  _method;
	PROTOCOL		_protocol;

	std::string		_path;
	vectorVar		_headers;
	vectorVar		_vars;

	std::string		_http;
	UnitDate		_startingLine;
	UnitDate		_head;
	UnitDate		_body;

	unsigned int		_parsePosition;
	unsigned int		_leftLoadBody;
	bool			_isKnowSize;
	bool			_isChunked;
	bool			_isDateSize;
	bool			_isEndTransfer;
	std::string		_tempChunked;

	PARSE_STATUS		_parseStatus;
	
	bool			_isKeepAlive;

	size_t			findNewLine(const std::string &request, const size_t &begin, size_t& delta);

	std::string		parsePath(const std::string &url);
	bool			parseHeader(const std::string &line);
	void			parseVars(const std::string &line);
	void			parseVar(const std::string &line);

	unsigned int		hexToDec(const std::string &hex);
    } ;
}

#endif /* HTTP_H */

