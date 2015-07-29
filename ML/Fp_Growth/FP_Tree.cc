#include "FP_Tree.h"

FP_Tree::FP_Tree()
{
}

FP_Tree::~FP_Tree()
{
}

void FP_Tree::setDataFilePath(const std::string& path)
{
  dataFile_ = path;
}

void FP_Tree::setDataFilePath(const char* path)
{
  dataFile_ = path;
}

void FP_Tree::setMinimumSupport(const int minimumSupport)
{
  minimumSupport_ = minimumSupport;
}

bool FP_Tree::LoadData()
{
  if (dataFile_.empty()) {
    return false;
  }
}

