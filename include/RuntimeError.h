#ifndef RUNTIMEERROR_H
#define RUNTIMEERROR_H

#include <cerrno>
#include <cstring>
#include <stdexcept>

namespace SiTNetwork
{
    struct RuntimeError : public std::runtime_error {
    public:
        explicit RuntimeError(const char *what_msg) : std::runtime_error(what_msg) {};
	explicit RuntimeError() : std::runtime_error(strerror(errno)) {};
};
    
}




#endif /* RUNTIMEERROR_H */

