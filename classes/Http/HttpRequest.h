#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "Http.h"

namespace doxyCraft
{
    class HttpRequest: public Http
    {
    public:
	HttpRequest();
	virtual		~HttpRequest();

	String* gen();
	bool		parseBody(const String &line);
    protected:
	bool		parseStartingLine(const String &line);
    } ;
}

#endif /* HTTPREQUEST_H */

