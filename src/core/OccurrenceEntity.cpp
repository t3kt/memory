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
  add(rangeFadeIn.set("Fade In", 1, 0, 4));
//  add(markerColor.set("Marker Color", ofFloatColor(1, .5, .25, 1)));
//  add(rangeColor.set("Range Color", ofFloatColor(.5, .5, .5, 0.2)));
//  add(connectorColor.set("Connector Color", ofFloatColor(.5, .5, .5, 1.0)));
  add(markerSize
      .setKey("markerSize")
      .setName("Marker Size")
      .setValueAndDefault(0.05)
      .setRange(0, 0.5));
}

shared_ptr<OccurrenceEntity> OccurrenceEntity::spawn(const OccurrenceEntity::Params &params, const Bounds& bounds, const State& state, const ColorTheme& colors) {
  ofVec3f pos = bounds.randomPoint();
  float radius = params.radius.getValue();
  return std::make_shared<OccurrenceEntity>(pos, radius, params, state, colors);
}

OccurrenceEntity::OccurrenceEntity(ofVec3f pos, float radius, const Params& params, const State& state, const ColorTheme& colors)
: ParticleObject(pos, params)
, _actualRadius(0)
, _originalRadius(radius)
, _params(params)
, _colors(colors)
, _startTime(state.time) {
}

void OccurrenceEntity::addObserver(shared_ptr<ObserverEntity> observer) {
  _connectedObservers[observer->id] = observer;
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
  auto i = _connectedObservers.find(id);
  if (i != _connectedObservers.end()) {
    _connectedObservers.erase(id);
    recalculateRadius();
  }
}

void OccurrenceEntity::update(const State &state) {
  if (hasConnectedObservers()) {
    _behaviors.update(*this, state);
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

float OccurrenceEntity::getFadeInPercent(const State &state) const {
  return ofMap(state.time - _startTime, 0, _params.rangeFadeIn.get(), 0, 1, true);
}

void OccurrenceEntity::handleDeath() {
  ofLogNotice() << "Occurrence died: " << *this;
}

void OccurrenceEntity::outputFields(std::ostream &os) const {
  ParticleObject::outputFields(os);
  os << ", originalRadius: " << _originalRadius
      << ", actualRadius: " << _actualRadius;
}
