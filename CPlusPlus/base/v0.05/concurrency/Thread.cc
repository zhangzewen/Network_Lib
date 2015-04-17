#include "Thread.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include <pthread.h>


static void* startThread(void* obj)
{
  Thread* thread = static_cast<Thread*>(obj);
	thread->run();
  return NULL;
}


Thread::Thread(const ThreadFunc func, const std::string& n)
  : started_(false),
    joined_(false),
    pthreadId_(0),
    func_(func),
    name_(n)
{
}

Thread::~Thread()
{
  if (started_ && !joined_)
  {
    pthread_detach(pthreadId_);
  }
}

void Thread::start()
{
  assert(!started_);
  started_ = true;

  if (pthread_create(&pthreadId_, NULL, &startThread, this))
  {
    started_ = false;
    abort();
  }
}

void Thread::join()
{
  assert(started_);
  assert(!joined_);
  joined_ = true;
  pthread_join(pthreadId_, NULL);
}
