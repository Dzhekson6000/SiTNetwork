#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H
#include "Http.h"
#include <map>

namespace doxyCraft
{
    class HttpResponse: public Http
    {
    public:
	typedef std::map<int, const char *> mapStatus;
	HttpResponse();
	virtual		    ~HttpResponse();

	String* gen();
	bool		    parseBody(const String &line);

	void		    setStatus(unsigned int status);
	void		    setBody(const String &body);

	unsigned int	    getStatus();
	static const char*  getStatus(int status);
    protected:
	unsigned int	    _status;
	static mapStatus    _statuses;
	String	    _bodyResponse;

	bool		    parseStartingLine(const String &line);
    } ;
}

#endif /* HTTPRESPONSE_H */

