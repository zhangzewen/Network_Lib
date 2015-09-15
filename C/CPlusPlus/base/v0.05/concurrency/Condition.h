#ifndef __CONDITION_H_INCLUDED__
#define __CONDITION_H_INCLUDED__
#include <assert.h>
#include <pthread.h>
#include "Mutex.h"

namespace concurrency
{

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

}
#endif //__CONDITION_H_INCLUDED__
