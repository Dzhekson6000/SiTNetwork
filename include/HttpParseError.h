#ifndef HTTPPARSEERROR_H
#define HTTPPARSEERROR_H

#include <cerrno>
#include <cstring>
#include <stdexcept>

namespace SiTNetwork
{
    struct HttpParseError : public std::runtime_error {
    public:
        explicit HttpParseError(const char *what_msg) : std::runtime_error(what_msg) {};
	explicit HttpParseError() : std::runtime_error(strerror(errno)) {};
};
    
}

#endif /* HTTPPARSEERROR_H */

