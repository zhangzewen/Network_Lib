#ifndef __NETWORKLIB_UTIL_FILEINPUTSTREAM_HPP_INCLUDED__
#define __NETWORKLIB_UTIL_FILEINPUTSTREAM_HPP_INCLUDED__

class FileInputStream
{
public:
	int available();
	void close();
	void finalize();
	Channel& getChannel();
	int getFD();
	int read();
	int read(uint8_t*);
	int read(uint8_t*, uint32_t, uint32_t);
};

#endif
