#include "event.h"

event::event() : fd_(-1), ready_(false), active_(false), timeout_(false),
  timerSet_(false), privData_(NULL)
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
