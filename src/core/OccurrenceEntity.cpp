//
//  OccurrenceEntity.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#include "OccurrenceEntity.h"
#include "ObserverEntity.h"
#include <ofMain.h>
#include <algorithm>

const float MAX_OBS_LEVEL = 4.0f;

OccurrenceEntity::Params::Params()
: ParticleObject::Params() {
  add(radius
      .setKey("radius")
      .setName("Radius Range")
      .setParamValuesAndDefaults(0.4, 1.3)
      .setParamRanges(0, 4));
}

shared_ptr<OccurrenceEntity> OccurrenceEntity::spawn(const OccurrenceEntity::Params &params, const Bounds& bounds, const State& state) {
  ofVec3f pos = bounds.randomPoint();
  float radius = params.radius.getValue();
  return std::make_shared<OccurrenceEntity>(pos, radius, params, state);
}

OccurrenceEntity::OccurrenceEntity(ofVec3f pos, float radius, const Params& params, const State& state)
: ParticleObject(pos, params, state)
, _actualRadius(0)
, _originalRadius(radius)
, _startTime(state.time) {
}

void OccurrenceEntity::addObserver(shared_ptr<ObserverEntity> observer) {
  _connectedObservers.add(observer);
  float dist = _position.distance(observer->position());
  if (dist > _actualRadius) {
    _actualRadius = dist;
  }
}

void OccurrenceEntity::recalculateRadius() {
  _actualRadius = 0;
  for (auto observer : _connectedObservers) {
    float dist = _position.distance(observer.second->position());
    if (dist > _actualRadius) {
      _actualRadius = dist;
    }
  }
}

void OccurrenceEntity::removeObserver(ObjectId id) {
  if (_connectedObservers.remove(id)) {
    recalculateRadius();
  }
}

void OccurrenceEntity::update(const State &state) {
  if (hasConnectedObservers()) {
    ParticleObject::update(state);
    recalculateRadius();
  } else {
    kill();
  }
}

float OccurrenceEntity::getAmountOfObservation(const State& state) const {
  float result = 0;
  for (auto observer : _connectedObservers) {
    result += observer.second->getRemainingLifetimeFraction();
  }
  return result;
}

void OccurrenceEntity::handleDeath() {
  ofLogNotice() << "Occurrence died: " << *this;
}

void OccurrenceEntity::outputFields(std::ostream &os) const {
  ParticleObject::outputFields(os);
  os << ", originalRadius: " << _originalRadius
      << ", actualRadius: " << _actualRadius;
}
