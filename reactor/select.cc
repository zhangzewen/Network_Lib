// Copyright [2015] <Zhang Zewen>
#include "select.h"

#include <assert.h>
#include "event.h"

#include <glog/logging.h>

#include "dispatcher.h"
#include "internal-define.h"

Select::Select()
{
}

Select::~Select()
{
}

bool Select::init()
{
  FD_ZERO(&readSet_);
  FD_ZERO(&writeSet_);
  return true;
}

void Select::poll(dispatcher* disp, void* arg)
{
  fd_set currentReadSet;
  fd_set currentWriteSet;

  currentReadSet = readSet_;
  currentWriteSet = writeSet_;

  int ret = select(maxFd_ + 1, &currentReadSet, &currentWriteSet, NULL, NULL);
  if (ret < 0) {
    LOG(ERROR) << "select error!";
  }
  if (ret == 0) {
    LOG(ERROR) << "no event fired until timeout";
  }

  //  iterator fired read event
  for (std::map<int, event*>::const_iterator iter = readEventMap_.begin();
    iter != readEventMap_.end();
    ++iter) {
    if (FD_ISSET(iter->first, &currentReadSet)) {
      event* ev = iter->second;
      ev->setReady(true);
      disp->registActiveEvent(ev);
    }
  }

  //  iterator fired write event
  for (std::map<int, event*>::const_iterator iter = writeEventMap_.begin();
    iter != writeEventMap_.end();
    ++iter) {
    if (FD_ISSET(iter->first, &currentWriteSet)) {
      event* ev = iter->second;
      ev->setReady(true);
      disp->registActiveEvent(ev);
    }
  }
}

int Select::addEvent(event* ev, int what, int flag)
{
  if (what & EV_READ) {
    FD_SET(ev->getFd(), &readSet_);
    readEventMap_[ev->getFd()] = ev;
  }
  if (what & EV_WRITE) {
    FD_SET(ev->getFd(), &writeSet_);
    writeEventMap_[ev->getFd()] = ev;
  }
  if (ev->getFd() >= maxFd_) {
    maxFd_ = ev->getFd();
  }
  return 0;
}

int Select::delEvent(event* ev, int what, int flag)
{
  if (what & EV_READ) {
    FD_CLR(ev->getFd(), &readSet_);
    std::map<int, event*>::iterator iter = readEventMap_.find(ev->getFd());
    if (iter != readEventMap_.end()) {
      readEventMap_.erase(iter);
    }
  }
  if (what & EV_WRITE) {
    FD_CLR(ev->getFd(), &writeSet_);
    std::map<int, event*>::iterator iter = writeEventMap_.find(ev->getFd());
    if (iter != writeEventMap_.end()) {
      writeEventMap_.erase(iter);
    }
  }
  return 0;
}

