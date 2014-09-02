#ifndef _NETWORKLIB_UTIL_BUFFER_HPP_INCLUDED_
#define _NETWORKLIB_UTIL_BUFFER_HPP_INCLUDED_

namespace NetworkLib{
namespace Util{

class Buffer
{
public:
	Buffer(): start_(NULL), end_(NULL), pos_(NULL), last_(NULL) {}
	~Buffer();
	int read(uint8_t*, uint32_t);
	int write(uint8_t*, uint32_t);
private:
	bool expend(uint32_t);
	void align();
	uint8_t* start_;
	uint8_t* end_;
	uint8_t* pos_;
	uint8_t* last_;
};
}
}


#endif

