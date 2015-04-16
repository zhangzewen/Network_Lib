#ifndef __MUTEX_H_INCLUDED__
#define __MUTEX_H_INCLUDED__

#include <assert.h>
#include <pthread.h>

namespace concurrency
{
class MutexLock : boost::noncopyable
{
	public:
		MutexLock() {
			pthread_mutex_init(&mutex_, NULL);
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
};

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
}


#endif
