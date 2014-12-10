#include <string>
#include <cstdio>
#include <errno.h>
#include <cstdio>
#include <string>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include "File.h"

namespace NetworkLib{
namespace Util{

using namespace std;

File::File(const std::string& pathname) : path_(pathname), isDirectory_(false),
	writeAble_(false), readAble_(false), exeAble_(false), isRegularFile_(false),
	isSymbolicFile_(false)
{

}

bool File::init() {
	int ret = 0;
	ret = stat(path_.c_str(), &stat_);
	if (ret < 0) {
		if (errno == EACCES) {
			fprintf(stderr, "Can't access!\n");
		} else if (errno == ENAMETOOLONG) {
			fprintf(stderr, "pathname too long!\n");
		}
	}

#if 0
	if (S_ISDIR(stat_.st_mode)) {
		isDirectory_ = true;
	}

	if (S_ISLNK(stat_.st_mode)) {
		isSymbolicFile_ = true;
	}

	if (S_ISREG(stat_.st_mode)) {
		isRegularFile_ = true;
	}
#endif

	if ((stat_.st_mode & S_IFMT) == S_IRUSR) {
		readAble_ = true;
	}

	if ((stat_.st_mode & S_IFMT) == S_IWUSR) {
		writeAble_ = true;
	}

	if ((stat_.st_mode & S_IFMT) == S_IXUSR) {
		exeAble_ = true;
	}
}
//File::File(const Path& pathname);
//File::File(std::string parent, std::string child)
//{
//}
bool File::canRead() {
	return readAble_;
}

bool File::canWrite() {
	return writeAble_;
}

bool File::canExec() {
	return exeAble_;
}

//int File::compareTo()
//{
	//return 0;
//}
//bool File::createNewFile()
//{
	//return true;
//}

//void File::Update()
//{
//}
//File& File::createTempFile(std::string prefix, std::string suffix)
//{
	//return *this;
//}
//File& File::createTempFile(std::string prefix, std::string suffix, const File& directory)
//{
	//return *this;
//}
//bool File::Delete()
//{
	//return true;
//}
//void File::deleteOnExit()
//{

//}
//bool File::exists()
//{
	//return true;
//}

//File& File::getAbsoluteFile()
//{
  //return *this;
//}
//std::string File::getAbsoluteFile()
//{
  //return "";
//}
//File& File::getCanonicalFile()
//{
  //return *this;
//}
//std::string File::getCanonicalFile()
//{
  //return "";
//}
std::string File::getName()
{
	return "";
}
std::string File::getParent()
{
	return "";
}
File& File::getParentFile()
{
	return *this;
}
std::string File::getPath()
{
	return "";
}
int File::hashCode()
{
	return 0;
}

bool File::isAbsolute()
{
	return true;
}

bool File::IsRegularFile()
{
	return isRegularFile_;
}

bool File::IsSymbolicFile()
{
	return isSymbolicFile_;
}


//bool File::isHidden()
//{
	//return true;
//}

//long File::lastModified()
//{
	//return 0L;
//}

long File::length()
{
	return 0L;
}

//std::string File::list()
//{
//	return "";
//}
//std::string list(FilenameFilter filter);
//File& File::listFiles()
//{
//	return *this;
//}
//File& listFiles(FileFilter filter);
//File& listFiles(FilenameFilter filter);
//File& File::LsItRoots()
//{
	//return *this;
//}
//bool File::mkdir()
//{
	//return true;
//}
//bool File::mkdirs()
//{
	//return true;
//}
bool File::renameTo()
{
	return true;
}
//bool File::setLastModified(long time)
//{
	////return true;
//}
//bool File::setReadOnly()
//{
	//return true;
//}
////std::string File::toString()
//{
	//return "";
//}

}
}

