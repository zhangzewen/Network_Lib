#include <execinfo.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "exception.hpp"

Exception::Exception(const char* msg) : message_(msg)
{
	fillStackTrace();
}

Exception::Exception(const std::string& msg) : message_(msg)
{
	fillStackTrace();
}

Exception::Exception(const char* file, int line, const char* what)
{
	char lines[32] = {0};
	sprintf(lines, "[%s : %d]", file, line);
	message_ += lines;
	message_.push_back(' ');
	message_ += what;
	message_ += "\n";
	fillStackTrace();
}

Exception::Exception(const char* file, int line, const string& what)
{
	char lines[5] = {0};
	sprintf(lines, "[%s : %d]", file, line);
	message_ += lines;
	message_.push_back(' ');
	message_ += what;
	message_ += "\n";
	fillStackTrace();
}

Exception::~Exception() throw ()
{
}

const char* Exception::what() const throw()
{
	return message_.c_str();
}

const char* Exception::stackTrace() const throw()
{
	return stack_.c_str();
}

void Exception::fillStackTrace()
{
	const int len = 200;
	void *buff[len];

	int nptrs = ::backtrace(buff, len);
	char** strings = ::backtrace_symbols(buff, nptrs);
	if (strings) {
		for (int i = 0; i < nptrs; ++i) {
			stack_.append(strings[i]);
			stack_.push_back('\n');
		}
		free(strings);
	}
}

std::string& Exception::get_Message()
{
	return message_;
}

