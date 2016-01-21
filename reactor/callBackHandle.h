#ifndef REACTOR_CALLBACKHANDLE_H_
#define REACTOR_CALLBACKHANDLE_H_

#include <functional>
#include <memory>

class Event;
typedef std::function<void(std::shared_ptr<Event>)> eventHandler;

#endif
