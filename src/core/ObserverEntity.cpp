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
: ::ParticleObject::Params("Observers")
, lifetime("Lifetime Range") {
  add(lifetime);
  lifetime.set(1, 4);
  lifetime.setParamRange(0, 240);
  add(color.set("Color", ofFloatColor::fromHsb(0.25, 0.5, 0.7, 1.0)));
  add(drawRadius.set("Draw Radius", 0.03, 0, 0.1));
}

ObserverOccurrenceAttraction::Params::Params()
: ::Params("Occurrence Attraction")
, distanceBounds("Distance Bounds")
, forceRange("Force Range") {
  add(distanceBounds.set(0.04, 0.3).setParamRange(0, 1));
  add(forceRange.set(0, 0.002).setParamRange(-0.04, 0.04));
}

void ObserverEntity::Params::initPanel(ofxGuiGroup &panel) {
//  panel.getGroup("Spawn Area").minimize();
//  panel.getGroup("Color").minimize();
}

shared_ptr<ObserverEntity> ObserverEntity::spawn(const ObserverEntity::Params &params, const Bounds& bounds, const State& state) {
  ofVec3f pos = bounds.randomPoint();
  float life = params.lifetime.getValue();
  return shared_ptr<ObserverEntity>(new ObserverEntity(pos, life, params, state));
}

ObserverEntity::ObserverEntity(ofVec3f pos, float life, const ObserverEntity::Params& params, const State& state)
: ParticleObject(pos, params)
, _startTime(state.time)
, _totalLifetime(life)
, _params(params)
{
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

void ObserverEntity::draw(const State &state) {
  float alpha = _lifeFraction;
  if (alpha <= 0) {
    return;
  }
  ofFloatColor color = _params.color.get();
  color.a *= alpha;
  
  ofPushStyle();
  ofFill();
  ofSetColor(color);
  ofDrawSphere(_position, _params.drawRadius.get());
  ofPopStyle();
}

void ObserverEntity::outputFields(std::ostream &os) const {
  ParticleObject::outputFields(os);
  os << ", startTime: " << _startTime
      << ", totalLifetime: " << _totalLifetime
      << ", lifeFraction: " << _lifeFraction;
}
