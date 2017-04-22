#ifndef HTTP_H
#define HTTP_H

#include <unordered_map>
#include "../../../Shared/Types/String/String.h"

typedef std::unordered_map<doxyCraft::String, doxyCraft::String> hashMap;


namespace doxyCraft
{
	class Http
	{
	public:
		
		struct UnitDate
		{
			unsigned int begin;
			unsigned int end;
		};
		enum class PROTOCOL:unsigned int
		{
			HTTP09 = 0,
			HTTP10 = 1,
			HTTP11 = 2,
			HTTP2  = 3,
			COUNT  = 4
		};
		enum class METHOD:unsigned int
		{
			OPTIONS       = 0,
			GET           = 1,
			HEAD          = 2,
			POST          = 3,
			PUT           = 4,
			DELETE_METHOD = 5,
			TRACE         = 6,
			CONNECT       = 7,
			COUNT         = 8
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
		virtual            ~Http();
		
		void clear();
		bool isKnowSize();
		void endTransfer();
		
		virtual String* gen();
		bool parse();
		bool parse(const String &request);
		
		bool parseNewDate(const String &date);
		bool parseNewChunked(const String &date);
		
		virtual bool parseStartingLine(const String &line);
		bool parseHead(const String &head);
		virtual bool parseBody(const String &body);
		
		void setMethod(METHOD method);
		void setPath(const String &path);
		void setProtocol(PROTOCOL protocol);
		void addHeader(const String &key, const String &value);
		void setHeaders(hashMap &headers);
		void addVar(const String &key, const String &value);
		
		const String* getHttp() const;
		const String getStartingLine() const;
		const String getHead() const;
		const String getBody() const;
		
		METHOD getMethod();
		String getPath() const;
		PROTOCOL getProtocol() const;
		
		String getHeader(const String &key) const;
		String getVar(const String &key) const;
		
		hashMap getHeaders() const;
		hashMap getVars() const;
		
		PARSE_STATUS getParseStatus();
		
		String getMethodAtString(METHOD method);
		METHOD getMethodFromString(const String &method);
		
		String getProtocolAtString(PROTOCOL protocol);
		PROTOCOL getProtocolFromString(const String &protocol);
		
		bool isKeepAlive() const;
		void setKeepAlive(bool isKeepAlive);
	protected:
		METHOD   _method;
		PROTOCOL _protocol;
		
		String _path;
		hashMap     _headers;
		hashMap     _vars;
		
		String _http;
		UnitDate    _startingLine;
		UnitDate    _head;
		UnitDate    _body;
		
		unsigned int _parsePosition;
		unsigned int _leftLoadBody;
		bool         _isKnowSize;
		bool         _isChunked;
		bool         _isDateSize;
		bool         _isEndTransfer;
		String  _tempChunked;
		
		PARSE_STATUS _parseStatus;
		
		bool _isKeepAlive;
		
		size_t findNewLine(const String &request, const size_t &begin, size_t &delta);
		
		String parsePath(const String &url);
		bool parseHeader(const String &line);
		void parseVars(const String &line);
		void parseVar(const String &line);
		
		unsigned int hexToDec(const String &hex);
	};
}

#endif /* HTTP_H */

