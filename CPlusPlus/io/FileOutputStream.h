#ifndef __NETWORKLIB_UTIL_FILEOUTPUTSTREAM_HPP_INCLUDED__
#define __NETWORKLIB_UTIL_FILEOUTPUTSTREAM_HPP_INCLUDED__

namespace NetworkLib{
namespace Util{

class FileOutputStream{
public:
	FileOutputStream(File file);
	FileOutputStream(File file, bool append);
	FileOutputStream(int fd);
	FileOutputStream(std::string name);
	FileOutputStream(std::string name, bool append);

	void close();
	void finalize();
	Channel& getChannel();
	int getFD();
	void write(uint8_t* buf);
	void write(uint8_t* buf, int , int);
	void write(int len);
};

}
}

#endif
