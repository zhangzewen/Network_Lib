#include "util.h"


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
