#ifndef _ML_APRIORI_H_INCLUDED_
#define _ML_APRIORI_H_INCLUDED_
class Apriori
{
public:
  void printTSet() const;
private:
  std::vector<std::vector<std::string> > TSet_;
};
#endif
