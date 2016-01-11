#ifndef REACTOR_CALLBACKHANDLE_H_
#define REACTOR_CALLBACKHANDLE_H_

#include <functional>

class Event;
typedef std::function<void(Event*)> eventHandler;

#endif
