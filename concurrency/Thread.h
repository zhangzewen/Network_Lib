// Copyright [2015] <Zhang Zewen>
#ifndef CONCURRENCY_THREAD_H_
#define CONCURRENCY_THREAD_H_

#include <pthread.h>
#include <string>

class Thread
{
 public:
  typedef void (*ThreadFunc)(void * data);
  explicit Thread(const ThreadFunc, const std::string& name = std::string());
  ~Thread();
  void start();
  void join();
  void run();
  bool started() const { return started_; }
  const std::string& name() const { return name_; }
 private:
  bool        started_;
  bool        joined_;
  pthread_t   pthreadId_;
  ThreadFunc  func_;
  std::string name_;
};


#endif  //  CONCURRENCY_THREAD_H_
