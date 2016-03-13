#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H
#include "Http/Http.h"
#include <map>

namespace SiTNetwork
{
    class HttpResponse: public Http
    {
    public:
	typedef std::map<int, const char *> mapStatus;
	HttpResponse();
	virtual		    ~HttpResponse();

	std::string*	    gen();
	bool		    parseBody(const std::string &line);

	void		    setStatus(unsigned int status);
	void		    setBody(const std::string &body);

	unsigned int	    getStatus();
	static const char*  getStatus(int status);
    protected:
	unsigned int	    _status;
	static mapStatus    _statuses;
	std::string	    _bodyResponse;

	bool		    parseStartingLine(const std::string &line);
    } ;
}

#endif /* HTTPRESPONSE_H */

