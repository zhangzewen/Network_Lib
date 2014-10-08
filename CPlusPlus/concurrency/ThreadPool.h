#ifndef YISIS_THREADPOOL_HPP_INCLUDED_
#define YISIS_THREADPOOL_HPP_INCLUDED_

#include <exception>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <deque>

#include "Thread.h"
#include "condition.h"
#include "mutex.h"

using namespace NetworkLib::concurrency;

namespace NetworkLib{
namespace concurrency{

class ThreadPool : boost::noncopyable
{
 public:
  typedef boost::function<void ()> Task;

  explicit ThreadPool(const std::string& name = std::string());
  ~ThreadPool();

  void start(int numThreads);
  void stop();

  void run(const Task& f);

 private:
  void runInThread();
  Task take();

  MutexLock mutex_;
  Condition cond_;
  std::string name_;
  boost::ptr_vector<Thread> threads_;
  std::deque<Task> queue_;
  bool running_;
};

#endif
}
}
