#ifndef _ML_FP_GROWTH_FP_TREE_H_INCLUDED_
#define _ML_FP_GROWTH_FP_TREE_H_INCLUDED_

class FP_Tree
{
public:
  bool LoadData(); 
  void setDataFilePath(const std::string& path);
  void setDataFilePath(const char* path);
  void setMinimumSupport(const int MinimumSupport);
private:
  std::string dataFile_;
  int minimumSupport_;
  std::vector<std::vector<std::string> > TSet_;
};
#endif //_ML_FP_GROWTH_FP_TREE_H_INCLUDED_
