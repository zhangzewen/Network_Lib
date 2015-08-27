#ifndef UTIL_NONCOPYABLE_H_
#define UTIL_NONCOPYABLE_H_

class noncopyable
{
public:
	noncopyable(){}
	~noncopyable(){}
private:
	noncopyable(const noncopyable&);
	noncopyable& operator=(const noncopyable&);

};
#endif  // UTIL_NONCOPYABLE_H_
