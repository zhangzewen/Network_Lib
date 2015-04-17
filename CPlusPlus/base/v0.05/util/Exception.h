#ifndef __EXCEPTION_H_INCLUDED__
#define __EXCEPTION_H_INCLUDED__

#include <exception>

namespace Util
{

class Exception : public std::exception
{
public:
	Exception() : message_(0), stack_(0) {}
	Exception(const char* what);
	Exception(const std::string& what);
	Exception(const char* file, int line, const char* what);
	Exception(const char* file, int line, const std::string& what);
	virtual ~Exception() throw();
	virtual const char* what() const throw();
	const char* stackTrace() const throw();
	std::string& get_Message();
private:
	void fillStackTrace();
	std::string message_;
	std::string stack_;
};

}
#endif
