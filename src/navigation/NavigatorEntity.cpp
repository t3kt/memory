//
//  NavigatorEntity.cpp
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#include "Info.h"
#include "NavigatorEntity.h"
#include "NavigatorState.h"

NavigatorEntity::NavigatorEntity(NavigatorStatePtr prevState,
                                 Context& context)
: ParticleObject(prevState->position())
, _prevState(prevState)
, _lastChangeTime(context.time()) { }

void NavigatorEntity::outputFields(std::ostream &os) const {
  ParticleObject::outputFields(os);
  os << ", prev: ";
  if (_prevState) {
    os << *_prevState;
  } else {
    os << "(none)";
  }
  os << ", next: ";
  if (_nextState) {
    os << *_nextState;
  } else {
    os << "(none)";
  }
}

void NavigatorEntity::fillInfo(Info &info) const {
  ParticleObject::fillInfo(info);
  info.add("targetpt:", targetPoint());
  if (!_prevState) {
    info.add("prev:", "(none)");
  } else {
    info.add("prev:", _prevState->typeName());
    _prevState->fillInfo(info, "prev.");
  }
  if (!_nextState) {
    info.add("next:", "(none)");
  } else {
    info.add("next:", _nextState->typeName());
    _nextState->fillInfo(info, "next.");
  }
}

const ofVec3f& NavigatorEntity::prevPosition() const {
  return _prevState->position();
}

const ofVec3f& NavigatorEntity::nextPosition() const {
  return _nextState->position();
}

void NavigatorEntity::updateNextState(Context &context) {
  if (!_prevState) {
    return;
  }
  _nextState = _prevState->nextState(context);
  _lastChangeTime = context.time();
}

void NavigatorEntity::reachNextState(Context &context) {
  _prevState = _nextState;
  _nextState.reset();
  _lastChangeTime = context.time();
}

const ofVec3f& NavigatorEntity::targetPoint() const {
  return _nextState ? _nextState->position() : _prevState->position();
}

bool NavigatorEntity::stateAlive() const {
  if (!_prevState) {
    return false;
  }
  return _nextState ? _nextState->alive() : _prevState->alive();
}
