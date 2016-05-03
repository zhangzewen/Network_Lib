// Copyright [2015] <Zhang Zewen>

#ifndef CONCURRENCY_CONDITION_H_
#define CONCURRENCY_CONDITION_H_

#include <assert.h>
#include <pthread.h>
#include <boost/noncopyable.hpp>
#include "mutex.h"


class Condition : public boost::noncopyable
{
 public:
  explicit Condition(MutexLock& lock) : mutex_(lock) {
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


#endif  //  CONCURRENCY_CONDITION_H_
