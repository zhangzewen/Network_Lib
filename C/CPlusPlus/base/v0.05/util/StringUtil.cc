#include "util.h"

namespace Util
{
std::string lstrip(const std::string& str, std::string decollator)
{
	std::string::size_type pos = str.find_first_not_of(decollator);
	if (std::string::npos == pos) {
		return str;
	}
	std::string tmp(str, pos, str.size());
	return tmp;
}

std::string rstrip(const std::string& str, std::string decollator)
{
	std::string::size_type pos = str.find_last_not_of(decollator);
	if (std::string::npos == pos) {
		return str;
	}
	std::string tmp(str, 0, pos);
	return tmp;
}

std::string strip(std::string& str, std::string decollator)
{
	return rstrip(lstrip(str, decollator), decollator);
}

void split(const std::string& str, const std::string& decollator,
		std::vector<std::string>& array)
{
	string tmp = str;
	string::size_type pos1, pos2;
	pos2 = tmp.find(decollator);
	pos1 = 0;
	while(string::npos != pos2) {
		string elem = tmp.substr(pos1, pos2 - pos1);
		array.push_back(elem);
		pos1 = pos2 + 1;
		pos2 = tmp.find(decollator, pos1);
	}

	if (pos1 < tmp.size()) {
		string elem = tmp.substr(pos1);
		array.push_back(elem);
	}
}
}
