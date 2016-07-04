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
: ::Params() {
  add(_paused
      .setKey("paused")
      .setName("Paused")
      .setValueAndDefault(false));
  add(_rate
      .setKey("rate")
      .setName("Rate")
      .setValueAndDefault(1)
      .setRange(0, 4));
}

Clock::Clock(Clock::Params& params, State& state)
: _params(params)
, _state(state) {
}

void Clock::setup() {
  _state.time = 0;
  _state.timeDelta = 0;
  STATUS_STATE = _status.registerLine("State:");
  STATUS_TIME = _status.registerLine("Time:");
}

void Clock::start() {
  _timer.start();
  _timer.lastCallTime = _timer.startTime;
}

void Clock::stop() {
  _timer.stop();
}

void Clock::toggleState() {
  _params.setPaused(!_params.paused());
}

void Clock::update() {
  if (_params.paused()) {
    if (_timer.isRunning) {
      stop();
    }
    _state.timeDelta = 0;
    _status.setValue(STATUS_STATE, "Paused");
  } else {
    if (!_timer.isRunning) {
      start();
    }
    float rawDelta = _timer.getSecondsSinceLastCall();
    float delta = rawDelta * _params.rate();
    _state.timeDelta = delta;
    _state.time += delta;
    _status.setValue(STATUS_STATE, "Playing");
  }
  _status.setValue(STATUS_TIME, ofToString(_state.time, 2));
}
