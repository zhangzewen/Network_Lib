#include "util.h"
#include <fcntl.h>
#include <iostream>
namespace Util
{
int setNonBlock(int fd)
{
	int ret;
	if ((ret = fcntl(fd, F_GETFL)) < 0) {
		std::cerr << "fnctl F_GETFL error!" << std::endl;
		return -1;
	}
	ret |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, ret) < 0) {
		std::cerr << "fcntl F_SETFL NONBLOCK errot!" << std::endl;
		return -1;
	}
	return 0;
}

StringTokenizer::StringTokenizer(const std::string& str, const std::string& token)
{
    if (0 == str.size()) {
        return;
    }

    std::string::size_type n = 0;
    std::string::size_type current = 0;
    std::string::size_type count = 0;
    n = str.find(token);
    while(n != std::string::npos && n < str.size()) {
        count = n - current;
        std::string tmp(str, current, count);
        strs_.push_back(tmp);
        n = str.find(str, current);
    }
    current_ = strs_.begin();
}

StringTokenizer::~StringTokenizer()
{
}

int StringTokenizer::countElements()
{
    return strs_.size();
}

std::string StringTokenizer::getNextElement()
{
    if (current_ != strs_.end()) {
        return *current_++;
    }

    return "";
}

std::string StringTokenizer::elementAt(int index)
{
    if (index > static_cast<int>(strs_.size()) || index < 0) {
        return "";
    }

    return strs_[index];
}

}
