//
//  Clock.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#include "Clock.h"
#include <ofMain.h>

Clock::Params::Params()
: ::Params("Clock") {
  add(paused.set("Paused", false));
  add(rate.set("Rate", 1, 0, 4));
}

Clock::Clock(Clock::Params& params, State& state)
: _params(params)
, _state(state) {
}

void Clock::setup() {
  _params.paused.addListener(this, &Clock::onPausedChanged);
  _state.time = 0;
  _state.timeDelta = 0;
  STATUS_STATE = _status.registerLine("State:");
  STATUS_TIME = _status.registerLine("Time:");
}

void Clock::start() {
  _timer.start();
  _timer.lastCallTime = _timer.startTime;
  _params.paused.setWithoutEventNotifications(false);
}

void Clock::stop() {
  _timer.stop();
  _params.paused.setWithoutEventNotifications(true);
}

void Clock::update() {
  if (_params.paused.get()) {
    _state.timeDelta = 0;
    _status.setValue(STATUS_STATE, "Paused");
  } else {
    float rawDelta = _timer.getSecondsSinceLastCall();
    float delta = rawDelta * _params.rate.get();
    _state.timeDelta = delta;
    _state.time += delta;
    _status.setValue(STATUS_STATE, "Playing");
  }
  _status.setValue(STATUS_TIME, ofToString(_state.time, 2));
}

void Clock::onPausedChanged(bool& paused) {
  if (paused) {
    stop();
  } else {
    start();
  }
}
