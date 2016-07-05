//
//  ObserverEntity.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include "State.h"
#include <ofMain.h>

ObserverEntity::Params::Params()
: ::ParticleObject::Params() {
  add(lifetime
      .setKey("lifetime")
      .setName("Lifetime Range")
      .setParamValuesAndDefaults(1, 4)
      .setParamRanges(0, 240));
}

shared_ptr<ObserverEntity> ObserverEntity::spawn(const ObserverEntity::Params &params, const Bounds& bounds, const State& state, const ofFloatColor& color) {
  ofVec3f pos = bounds.randomPoint();
  float life = params.lifetime.getValue();
  return std::make_shared<ObserverEntity>(pos, life, params, state, color);
}

ObserverEntity::ObserverEntity(ofVec3f pos, float life, const ObserverEntity::Params& params, const State& state, const ofFloatColor& color)
: ParticleObject(pos, params)
, _startTime(state.time)
, _totalLifetime(life)
, _params(params)
, _color(color)
, _lifeFraction(1) {
}

void ObserverEntity::addOccurrence(shared_ptr<OccurrenceEntity> occurrence) {
  _connectedOccurrences.push_back(occurrence);
}

void ObserverEntity::update(const State &state) {
  float elapsed = state.time - _startTime;
  if (elapsed > _totalLifetime) {
    _lifeFraction = 0.0f;
    kill();
  } else {
    _lifeFraction = ofMap(elapsed, 0.0f, _totalLifetime, 1.0f, 0.0f);
    _behaviors.update(*this, state);
    ParticleObject::update(state);
  }
}

void ObserverEntity::handleDeath() {
  ofLogNotice() << "Observer died: " << *this;
  for (auto occurrence : _connectedOccurrences) {
    occurrence->removeObserver(id);
  }
}

void ObserverEntity::outputFields(std::ostream &os) const {
  ParticleObject::outputFields(os);
  os << ", startTime: " << _startTime
      << ", totalLifetime: " << _totalLifetime
      << ", lifeFraction: " << _lifeFraction;
}
