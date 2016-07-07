//
//  Clock.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#include "Clock.h"
#include <ofMain.h>


Clock::Clock(Clock::Params& params, State& state)
: _params(params)
, _state(state) {
}

void Clock::setup() {
  _lastTime = ofGetElapsedTimef();
  _isRunning = true;
  _state.time = 0;
  _state.timeDelta = 0;
}

void Clock::start() {
  _lastTime = ofGetElapsedTimef();
  _isRunning = true;
}

void Clock::stop() {
  _isRunning = false;
}

void Clock::toggleState() {
  _params.setPaused(!_params.paused());
}

void Clock::update() {
  if (_params.paused()) {
    if (_isRunning) {
      stop();
    }
    _state.timeDelta = 0;
  } else {
    if (!_isRunning) {
      start();
    }
    float nowTime = ofGetElapsedTimef();
    float rawDelta = nowTime - _lastTime;
    float delta = rawDelta * _params.rate();
    _state.timeDelta = delta;
    _state.time += delta;
    _lastTime = nowTime;
  }
}
