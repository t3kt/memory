//
//  Clock.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#include "Clock.h"
#include <ofMain.h>

class _Timer2 {
public:
  _Timer2(Clock::Params& params);

  double getTotalSeconds();  // elapsed seconds since beginning of time
  double getAppSeconds(); // elapsed seconds since start of app
  double getSecondsSinceLastCall(); // elapsed seconds since last time this function was called

  void start();
  void stop();
  double getSeconds(); // elapsed seconds since you called start()
  bool isRunning() const { return _isRunning; }
private:
  double _appStartTime;
  double _lastCallTime;
  double _startTime;
  double _stopTime;
  bool _isRunning;
  Clock::Params& _params;

  friend void HACK_toSetLastCallTimeToStartTime(_Timer2& timer);
};

_Timer2::_Timer2(Clock::Params& params)
: _params(params) {
  _appStartTime = ofGetElapsedTimef();
  _startTime = _lastCallTime = _appStartTime;
  _isRunning = true;
}

double _Timer2::getTotalSeconds() {
  return ofGetElapsedTimef();
}

double _Timer2::getAppSeconds() {
  return getTotalSeconds() - _appStartTime;
}

double _Timer2::getSecondsSinceLastCall() {
  float nowTime = getTotalSeconds();
  float diff = nowTime - _lastCallTime;
  _lastCallTime = nowTime;
  return diff;
}

void _Timer2::start() {
  _isRunning = true;
  _startTime = getTotalSeconds();
}

void _Timer2::stop() {
  _stopTime = getTotalSeconds();
  _isRunning = false;
}

double _Timer2::getSeconds() {
  if (_isRunning) {
    return getTotalSeconds() - _startTime;
  } else {
    return _stopTime - _startTime;
  }
}

void HACK_toSetLastCallTimeToStartTime(_Timer2& timer) {
  timer._lastCallTime = timer._startTime;
}

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
  _timer2 = std::make_shared<_Timer2>(params);
}

void Clock::setup() {
  _state.time = 0;
  _state.timeDelta = 0;
  STATUS_STATE = _status.registerLine("State:");
  STATUS_TIME = _status.registerLine("Time:");
}

void Clock::start() {
  _timer2->start();
  HACK_toSetLastCallTimeToStartTime(*_timer2);
}

void Clock::stop() {
  _timer2->stop();
}

void Clock::toggleState() {
  _params.setPaused(!_params.paused());
}

void Clock::update() {
  if (_params.paused()) {
    if (_timer2->isRunning()) {
      stop();
    }
    _state.timeDelta = 0;
    _status.setValue(STATUS_STATE, "Paused");
  } else {
    if (!_timer2->isRunning()) {
      start();
    }
    float rawDelta = _timer2->getSecondsSinceLastCall();
    float delta = rawDelta;// * _params.rate();
    _state.timeDelta = delta;
    _state.time += delta;
    _status.setValue(STATUS_STATE, "Playing");
  }

  _status.setValue(STATUS_TIME, ofToString(_state.time, 2));
}
