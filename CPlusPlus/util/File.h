#ifndef _NETWORKLIB_UTIL_FILE_HPP_INCLUDED_
#define _NETWORKLIB_UTIL_FILE_HPP_INCLUDED_
#include <string>
#include <cstdio>
#include <string>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include "Path.h"

using namespace std;
namespace NetworkLib{
namespace Util{

class File
{
public:
	File(const std::string& pathname);
	//File(const Path& pathname);
	File(std::string parent, std::string child);
	bool canRead();
	bool canWrite();
	bool canExec();
	int compareTo();
	bool createNewFile();
	File& createTempFile(std::string prefix, std::string suffix);
	File& createTempFile(std::string prefix, std::string suffix, const File& directory);
	bool Delete();
	void deleteOnExit();
	bool exists();
	File& getAbsoluteFile();
	//std::string getAbsoluteFile();
	File& getCanonicalFile();
	//std::string getCanonicalFile();
	std::string getName();
	std::string getParent();
	File& getParentFile();
	std::string getPath();
	int hashCode();
	bool isAbsolute();
	bool IsRegularFile();
	bool IsSymbolicFile();
	bool isHidden();
	long lastModified();
	long length();
	std::string list();
	//std::string list(FilenameFilter filter);
	File& listFiles();
	//File& listFiles(FileFilter filter);
	//File& listFiles(FilenameFilter filter);
	File& LsItRoots();
	bool mkdir();
	bool mkdirs();
	bool renameTo();
	bool setLastModified(long time);
	bool setReadOnly();
	void Update();
	std::string toString();
	bool IsDirectory() const {
		return IsDirectory_;
	}
private:
	std::string pathSeparator_;
	uint8_t pathSparatorChar_;
	std::string separator_;
	uint8_t separatorChar();
	std::string path_;
	int fd_;
	struct stat stat_;
	//proto
	bool IsDirectory_;
	bool IsWriteAble_;
	bool IsReadAble_;
	bool IsExeAble_;
	bool IsRegularFile_;
	bool IsSymbolicFile_;
};

}
}


#endif

