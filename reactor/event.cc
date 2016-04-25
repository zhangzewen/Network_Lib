// Copyright [2015] <Zhang Zewen>
#include "event.h"

Event::~Event()
{
    fd_ = -1;
    ready_ = false;
    active_ = false;
    timeout_ = false;
}
std::string Event::getName() const {
    return name_;
}
