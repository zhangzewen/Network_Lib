#include <string>
#include <cstdio>
#include "File.h"
#include <iostream>

namespace NetworkLib{
namespace Util{

using namespace std;

File::File(const std::string& pathname) : path_(pathname)
{
	stat(path_, &stat_);
}
//File::File(const Path& pathname);
File::File(std::string parent, std::string child)
{
}
bool File::canRead()
{
	return true;
}
bool File::canWrite()
{
	return true;
}
int File::compareTo()
{
	return 0;
}
bool File::createNewFile()
{
	return true;
}

void File::Update()
{
}
File& File::createTempFile(std::string prefix, std::string suffix)
{
	return *this;
}
File& File::createTempFile(std::string prefix, std::string suffix, const File& directory)
{
	return *this;
}
bool File::Delete()
{
	return true;
}
void File::deleteOnExit()
{

}
bool File::exists()
{
	return true;
}
/*
 *File& File::getAbsoluteFile()
 *{
 *  return *this;
 *}
 *std::string File::getAbsoluteFile()
 *{
 *  return "";
 *}
 *File& File::getCanonicalFile()
 *{
 *  return *this;
 *}
 *std::string File::getCanonicalFile()
 *{
 *  return "";
 *}
 */
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
bool File::isDirectory()
{
	return true;
}
bool File::isFile()
{
	return true;
}
bool File::isHidden()
{
	return true;
}
long File::lastModified()
{
	return 0L;
}
long File::length()
{
	return 0L;
}
std::string File::list()
{
	return "";
}
//std::string list(FilenameFilter filter);
File& File::listFiles()
{
	return *this;
}
//File& listFiles(FileFilter filter);
//File& listFiles(FilenameFilter filter);
File& File::LsItRoots()
{
	return *this;
}
bool File::mkdir()
{
	return true;
}
bool File::mkdirs()
{
	return true;
}
bool File::renameTo()
{
	return true;
}
bool File::setLastModified(long time)
{
	return true;
}
bool File::setReadOnly()
{
	return true;
}
std::string File::toString()
{
	return "";
}

}
}

