
#ifndef YISIS_THREAD_HPP_INCLUDED_
#define YISIS_THREAD_HPP_INCLUDED_


#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <pthread.h>

extern int terminate;

class Thread : boost::noncopyable
{
 public:
  typedef boost::function<void ()> ThreadFunc;
  explicit Thread(const ThreadFunc&, const std::string& name = std::string());
  ~Thread();
  void start();
  void join();
	void run() {func_();}
  bool started() const { return started_; }
  const std::string& name() const { return name_; }
 private:
  bool        started_;
  bool        joined_;
  pthread_t   pthreadId_;
  ThreadFunc  func_;
  std::string name_;
};

#endif
