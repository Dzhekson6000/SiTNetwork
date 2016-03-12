#ifndef HTTP_H
#define HTTP_H

#include <string>
#include <vector>

namespace SiTNetwork
{
    class Http
    {
    public:
	enum class PROTOCOL : unsigned int
	{
	    HTTP09 = 0,
	    HTTP10 = 1,
	    HTTP11 = 2,
	    HTTP2 = 3,
            COUNT=4
	};
	
        enum class METHOD : unsigned int
        {
            OPTIONS=0,
            GET=1,
            HEAD=2,
            POST=3,
            PUT=4,
            DELETE=5,
            TRACE=6,
            CONNECT=7,
            COUNT=8
        };
        
	enum class PARSE_STATUS
	{
	    PARSE_STARTLINE,
	    PARSE_HEAD,
	    PARSE_BODY,
	    PARSE_END
	};
	
	static const char* PROTOCOL_STRING[];
	static const char* METHOD_STRING[];
	
        Http();
        virtual ~Http();

	void clear();
	
        virtual std::string* gen();
	bool parse();
        bool parse(const std::string &request);
	void parseNewDate(const std::string &date);
	
	virtual bool parseStartingLine(const std::string &line);
	bool parseHead(const std::string &head);
        virtual bool parseBody(const std::string& body);
	
        void setMethod(METHOD method);
        void setPath(const std::string &path);
        void setProtocol(PROTOCOL protocol);
        void setBody(const std::string &body);
        void addHeader(std::string key, std::string value);
        void setHeaders(std::vector<std::pair<std::string, std::string>>& headers);
        void addVar(std::string key, std::string value);
        
	const std::string* getHttp() const;
	const std::string* getBody() const;
        
	METHOD getMethod();
        std::string getPath() const;
        PROTOCOL getProtocol() const;
        
	std::string getHeader(std::string key) const;
	std::string getVar(std::string key) const;
	
        std::vector<std::pair<std::string, std::string>> getHeaders() const;
        std::vector<std::pair<std::string, std::string>> getVars() const;
        
	
        std::string getMethodAtString(METHOD method);
        METHOD getMethodFromString(const std::string& method);
	
	std::string getProtocolAtString(PROTOCOL protocol);
	PROTOCOL getProtocolFromString(const std::string& protocol);
    protected:
        METHOD _method;
	PROTOCOL _protocol;
	
        std::string _path;
        std::vector< std::pair<std::string, std::string>> _headers;
        std::vector< std::pair<std::string, std::string>> _vars;
        
        std::string _http;
        std::string _body;
	
	unsigned int _parsePosition;
	unsigned int _leftLoadDate;
	PARSE_STATUS _parseStatus;
	
        size_t findNewLine(const std::string &request, const size_t &begin, size_t& delta);
        
        std::string parsePath(const std::string &url);
        bool parseHeader(const std::string &line);
        void parseVars(const std::string &line);
        void parseVar(const std::string &line);
    };
}

#endif /* HTTP_H */

