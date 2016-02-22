#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <stdexcept>

class Exception : public std::exception {
    public:
        Exception() : mWhat("") {}
        Exception(const char * what) : mWhat(what) {}

        virtual const char * what() const throw() {
            return mWhat;
        }

    private:
        const char * mWhat;
};

#endif //ERICS3DENGINE_EXCEPTION_HPP
