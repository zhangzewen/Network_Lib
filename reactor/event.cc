// Copyright [2015] <Zhang Zewen>
#include "event.h"

event::event() : fd_(-1), ready_(false), active_(false), timeout_(false),
  timerSet_(false), registEvents_(0x0), privData_(NULL)
{
}

event::~event()
{
  privData_ = NULL;
  ready_ = false;
  active_ = false;
  timeout_ = false;
  timerSet_ = false;
}
