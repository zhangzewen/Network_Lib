#ifndef _UTIL_MUTEX_HPP_INCLUDED_
#define _UTIL_MUTEX_HPP_INCLUDED_

#include <boost/noncopyable.hpp>
#include <assert.h>
#include <pthread.h>

namespace NetworkLib{
namespace concurrency{
class MutexLock : boost::noncopyable
{
public:
	MutexLock() {
		pthread_mutex_init(&mutex_, NULL);
#ifdef DEBUG
		pthread_mutexattr_init(&mutexattr_);
		pthread_mutexattr_settype(&mutexattr_, PTHREAD_MUTEX_ERRORCHECK);
#endif
	}

	~MutexLock() {
		pthread_mutex_destroy(&mutex_);
	}

	void lock() {
		pthread_mutex_lock(&mutex_);
	}

	void unlock() {
		pthread_mutex_unlock(&mutex_);
	}

	bool trylock() {
		return pthread_mutex_trylock(&mutex_) == 0;
	}

	pthread_mutex_t* getMutexLock(){
		return &mutex_;
	}
private:
	pthread_mutex_t mutex_;
#ifdef DEBUG
	pthread_mutexattr_t mutexattr_;
#endif
};
}
}

using namespace NetworkLib::concurrency;

class MutexLockGuard : public boost::noncopyable
{
public:
	explicit MutexLockGuard(MutexLock& lock) : mutex_(lock) {
		mutex_.lock();
	}

	~MutexLockGuard() {
		mutex_.unlock();
	}
private:
	MutexLock& mutex_;

};
#endif
