//
//  Scheduler.cpp
//  memory
//
//  Created by tekt on 9/5/16.
//
//

#include "../core/Scheduler.h"

int IntervalScheduler::query() {
  if (!checkEnabled()) {
    return 0;
  }
  if (!checkChance()) {
    return 0;
  }
  bool wasInitial = _nextTime < 0;
  auto nowTime = _context.time();
  if (wasInitial || nowTime >= _nextTime) {
    _nextTime = nowTime + _params.interval.getValue();
    return wasInitial ? 0 : 1;
  } else {
    return 0;
  }
}

int RateScheduler::query() {
  if (!checkEnabled() || !checkChance()) {
    _lastTime = -1;
    return 0;
  }
  float now = _context.time();
  if (_lastTime < 0) {
    _lastTime = now;
    return 0;
  }
  float elapsed = now - _lastTime;
  float count = elapsed * _params.rate.get();
  if (count < 1) {
    return 0;
  }
  _lastTime = now;
  return static_cast<int>(count);
}
