#ifndef __BLOCKINGQUEUE_H_INCLUDED__
#define __BLOCKINGQUEUE_H_INCLUDED__

#include <deque>
#include <assert.h>
namespace concurrency
{

template<typename T>
class BlockingQueue : boost::noncopyable
{
public:
	BlockingQueue()
		: mutex_(),
		notEmpty_(mutex_),
		queue_()
    {
    }

	void put(const T& x)
	{
		MutexLockGuard lock(mutex_);
		queue_.push_back(x);
		notEmpty_.notify();
	}

	T take()
	{
		MutexLockGuard lock(mutex_);
		while (queue_.empty())
		{
			notEmpty_.wait();
		}
		assert(!queue_.empty());
		T front(queue_.front());
		queue_.pop_front();
		return front;
	}

	size_t size() const
	{
		MutexLockGuard lock(mutex_);
		return queue_.size();
	}

private:
	MutexLock mutex_;
	Condition notEmpty_;
	std::deque<T> queue_;
};

}


#endif //__BLOCKINGQUEUE_H_INCLUDED__
