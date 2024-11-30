#pragma once
#include <exception>
#include <string>

namespace Arrangement {

class ArrangementException : public std::exception {
    public:
        ArrangementException(const std::string& description) :
            exception(), m_description(description) {}
        virtual ~ArrangementException() throw() {}

    public:
        virtual const char* what() const throw() {
            return m_description.c_str();
        }

    private:
        std::string m_description;
};

class IOError : public ArrangementException {
    public:
        IOError(const std::string& description) :
            ArrangementException(description) {}
        virtual ~IOError() throw() {}
};

class RuntimeError : public ArrangementException {
    public:
        RuntimeError(const std::string& description) :
            ArrangementException(description) {}
        virtual ~RuntimeError() throw() {}
};

class NotImplementedError : public ArrangementException {
    public:
        NotImplementedError(const std::string& description) :
            ArrangementException(description) {}
        virtual ~NotImplementedError() throw() {}
};
}
