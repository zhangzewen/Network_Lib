// Copyright [2015] <Zhang Zewen>
#include "event.h"

Event::~Event()
{
    fd_ = -1;
    privData_ = NULL;
    ready_ = false;
    active_ = false;
    timeout_ = false;
    timerSet_ = false;
}
