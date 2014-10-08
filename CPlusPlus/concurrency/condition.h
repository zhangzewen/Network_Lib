#ifndef UTIL_CONDITION_HPP_INCLUDED_
#define UTIL_CONDITION_HPP_INCLUDED_

#include <boost/noncopyable.hpp>
#include <assert.h>
#include <pthread.h>
#include "mutex.h"

namespace NetworkLib{
namespace concurrency{

class Condition : public boost::noncopyable
{
public:
	Condition(MutexLock& lock) : mutex_(lock) {
		pthread_cond_init(&cond_, NULL);
	}

	~Condition() {
		pthread_cond_destroy(&cond_);
	}

	void wait() {
		pthread_cond_wait(&cond_, mutex_.getMutexLock());
	}

	void notify() {
		pthread_cond_signal(&cond_);
	}

	void notifyAll() {
		pthread_cond_broadcast(&cond_);
	}

private:
	MutexLock& mutex_;
	pthread_cond_t cond_;
};

#endif
}
}
