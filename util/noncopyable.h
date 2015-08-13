#ifndef UTIL_NONCOPYABLE_H
#define UTIL_NONCOPYABLE_H

class noncopyable
{
public:
	noncopyable(){}
	~noncopyable(){}
private:
	noncopyable(const noncopyable&);
	noncopyable& operator=(const noncopyable&);

};
#endif // UTIL_NONCOPYABLE_H
