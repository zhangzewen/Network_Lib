// Copyright [2015] <Zhang Zewen>
#include "event.h"

Event::Event() : fd_(-1), ready_(false), active_(false), timeout_(false),
  timerSet_(false), registEvents_(0x0), privData_(NULL)
{
}

Event::~Event()
{
  privData_ = NULL;
  ready_ = false;
  active_ = false;
  timeout_ = false;
  timerSet_ = false;
}
