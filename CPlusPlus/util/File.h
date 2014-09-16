#ifndef _NETWORKLIB_UTIL_FILE_HPP_INCLUDED_
#define _NETWORKLIB_UTIL_FILE_HPP_INCLUDED_
#include <string>
#include <cstdio>

namespace NetworkLib {
namespace Util {

class File
{
public:
	File(std::string pathname) : path_(pathname){}
	File(std::string parent, std::string child)
	{
		path_ = parent + child;
	}
	bool canRead();
	bool canWrite();
	int compareTo();
	bool createNewFile();
	File& createTempFile(std::string prefix, std::string suffix);
	File& createTempFile(std::string prefix, std::string suffix, const File& directory);
	boold delete();
	void deleteOnExit();
	bool exists();
	File& getAbsoluteFile();
	std::string getAbsoluteFile();
	File& getCanonicalFile();
	std::string getCanonicalFile();
	std::string getName();
	std::string getParent();
	File& getParentFile();
	std::string getPath();
	int hashCode();
	bool isAbsolute();
	bool isDirectory();
	bool isFile();
	bool isHidden();
	long lastModified();
	long length();
	std::string list();
	//std::string list(FilenameFilter filter);
	File& listFiles();
	//File& listFiles(FileFilter filter);
	//File& listFiles(FilenameFilter filter);
	File& lsitRoots();
	bool mkdir();
	bool mkdirs();
	bool renameTo();
	bool setLastModified(long time);
	bool setReadOnly();
	std::string toString();
	URI toURI();
	URL toURL();
private:
	std::string pathSeparator;
	uint8_t pathSparatorChar;
	std::string separator;
	uint8_t separatorChar();
	std::string path_;
};
}
}

#endif

