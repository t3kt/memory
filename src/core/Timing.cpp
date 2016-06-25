//
//  Timing.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include "Timing.h"
#include <ofMath.h>

bool OnceAction::update(float time) {
  if (_called)
    return true;
  if (time < _triggerTime)
    return false;
  this->call(time);
  _called = true;
  return true;
}

class FunctionOnceAction : public OnceAction {
public:
  FunctionOnceAction(float triggerTime, TimedFunction fn)
  : OnceAction(triggerTime)
  , _function(fn) {}
  
  void call(float time) override {
    _function(time);
    _called = true;
  }
private:
  const TimedFunction _function;
};

std::shared_ptr<OnceAction> OnceAction::newOnceAction(float triggerTime, TimedFunction fn) {
  return std::make_shared<FunctionOnceAction>(triggerTime, fn);
}


bool DurationAction::update(float time) {
  if (_ended)
    return true;
  if (!_started) {
    if (time >= _startTime)
      this->start();
    else
      return false;
  } else {
    if (time >= _endTime) {
      this->end();
      return true;
    }
  }
  float percentage = ofMap(time, _startTime, _endTime, 0, 1);
  call(time, percentage);
  return false;
}

class FunctionDurationAction : public DurationAction {
public:
  FunctionDurationAction(float start, float end, TimedPercentageFunction fn)
  : DurationAction(start, end), _function(fn) { }
  virtual void call(float time, float percentage) override {
    _function(time, percentage);
  }
private:
  const TimedPercentageFunction _function;
};

std::shared_ptr<DurationAction>
DurationAction::newDurationAction(float start, float end, TimedPercentageFunction fn) {
  return std::make_shared<FunctionDurationAction>(start, end, fn);
}

bool TimedActionSet::done() const {
  for (const std::shared_ptr<TimedAction>& action : _actions) {
    if (action && !action->done())
      return false;
  }
  return true;
}

bool TimedActionSet::update(float time) {
  bool allDone = true;
  for (auto i = _actions.begin();
       i != _actions.end(); ) {
    std::shared_ptr<TimedAction>& action = *i;
    bool done = false;
    if (!action) {
      done = true;
    } else {
      action->update(time);
      if (action->done())
        done = true;
    }
    if (done && _autoRemove) {
      i = _actions.erase(i);
    } else {
      i++;
    }
    if (!done)
      allDone = false;
  }
  return allDone;
}

void TimedActionSet::clear() {
  _actions.clear();
}
