//
//  Clock.cpp
//
//

#include <ofUtils.h>
#include "../core/Clock.h"

const float maxTimeStep = 1/30.0f;

float ClockNode::getEffectiveRate() const {
  if (isPaused()) {
    return 0;
  }
  auto rate = _params.rate.get();
  if (_parentClock) {
    return rate * _parentClock->getEffectiveRate();
  }
  return rate;
}

bool ClockNode::isPaused() const {
  if (_params.paused.get()) {
    return true;
  }
  if (_parentClock) {
    return _parentClock->isPaused();
  }
  return false;
}

void ClockNode::setPaused(bool paused) {
  _params.paused.set(paused);
  // when unpausing, also unpause the parent clock
  if (!paused && _parentClock) {
    _parentClock->setPaused(false);
  }
}

void ClockNode::toggleState() {
  setPaused(!_params.paused.get());
}

void ClockNode::start() {
  _lastAbsoluteTime = ofGetElapsedTimef();
  _state.running = true;
}

void ClockNode::stop() {
  _state.running = false;
  _state.rate = 0;
  _state.timeDelta = 0;
}

void ClockNode::setup() {
  _lastAbsoluteTime = ofGetElapsedTimef();
}

void ClockNode::update() {
  if (isPaused()) {
    if (_state.running) {
      stop();
    }
  } else {
    if (!_state.running) {
      start();
    }
    auto absNowTime = ofGetElapsedTimef();
    auto rawDelta = absNowTime = _lastAbsoluteTime;
    if (rawDelta > maxTimeStep) {
      rawDelta = maxTimeStep;
    }
    auto rate = getEffectiveRate();
    auto delta = rawDelta * rate;
    _state.timeDelta = delta;
    _state.localTime += delta;
    _state.rate = rate;
    _lastAbsoluteTime = absNowTime;
  }
}
