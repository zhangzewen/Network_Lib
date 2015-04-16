#ifndef __NONCOPYABLE_H_INCLUDED__
#define __NONCOPYABLE_H_INCLUDED__

namespace Util
{

class noncopyable
{
public:
	noncopyable(){}
	~noncopyable(){}
private:
	noncopyable(const noncopyable&);
	noncopyable& operator=(const noncopyable&);

};

}
#endif //__NONCOPYABLE_H_INCLUDED__
