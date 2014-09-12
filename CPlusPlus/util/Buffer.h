#ifndef _NETWORKLIB_UTIL_BUFFER_HPP_INCLUDED_
#define _NETWORKLIB_UTIL_BUFFER_HPP_INCLUDED_
#include <stdint.h>
#include <iostream>
#include <stdio.h>

namespace NetworkLib{
namespace Util{

class Buffer
{
public:
	Buffer(): start_(NULL), end_(NULL), pos_(NULL), last_(NULL) {}
	~Buffer();
	ssize_t read(int, uint8_t*, uint32_t);
	ssize_t write(int, uint8_t*, uint32_t);
private:
	int expend(size_t);
	int align();
	void drain(size_t);
	uint8_t* start_;
	uint8_t* end_;
	uint8_t* pos_;
	uint8_t* last_;
	size_t off_;
	size_t totallen_;
	size_t missalign_;
};
}
}


#endif

