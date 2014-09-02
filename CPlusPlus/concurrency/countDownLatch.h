
#ifndef MUDUO_BASE_COUNTDOWNLATCH_H
#define MUDUO_BASE_COUNTDOWNLATCH_H

#include "mutex.hpp"
#include "condition.hpp"

#include <boost/noncopyable.hpp>

class CountDownLatch : boost::noncopyable
{
 public:

  explicit CountDownLatch(int count)
    : mutex_(),
      condition_(mutex_),
      count_(count)
  {
  }

  void wait()
  {
    MutexLockGuard lock(mutex_);
    while (count_ > 0)
    {
      condition_.wait();
    }
  }

  void countDown()
  {
    MutexLockGuard lock(mutex_);
    --count_;
    if (count_ == 0)
    {
      condition_.notifyAll();
    }
  }

  int getCount() const
  {
    MutexLockGuard lock(mutex_);
    return count_;
  }

 private:
  mutable MutexLock mutex_;
  Condition condition_;
  int count_;
};
#endif  // MUDUO_BASE_COUNTDOWNLATCH_H
