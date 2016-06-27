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
: ::Params("Observers")
, lifetime("Lifetime Range")
, spawnArea("Spawn Area") {
  add(lifetime);
  lifetime.set(1, 4);
  lifetime.setParamRange(0, 240);
  add(spawnArea
      .set(ofVec3f(-1), ofVec3f(1))
      .setParamRange(ofVec3f(-2), ofVec3f(2)));
  add(color.set("Color", ofFloatColor::fromHsb(0.25, 0.5, 0.7, 1.0)));
  add(drawRadius.set("Draw Radius", 0.03, 0, 0.1));
}

void ObserverEntity::Params::initPanel(ofxGuiGroup &panel) {
//  panel.getGroup("Spawn Area").minimize();
//  panel.getGroup("Color").minimize();
}

shared_ptr<ObserverEntity> ObserverEntity::spawn(const ObserverEntity::Params &params, const State& state) {
  ofVec3f pos = params.spawnArea.getValue();
  float life = params.lifetime.getValue();
  return shared_ptr<ObserverEntity>(new ObserverEntity(pos, life, params, state));
}

ObserverEntity::ObserverEntity(ofVec3f pos, float life, const ObserverEntity::Params& params, const State& state)
: StandardWorldObject()
, _startTime(state.time)
, _totalLifetime(life)
, _params(params)
{
  position = pos;
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
  }
  _behaviors.update(*this, state);
}

void ObserverEntity::handleDeath() {
  std::cout << "Observer died: " << *this << std::endl;
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
  ofDrawSphere(position, _params.drawRadius.get());
  ofPopStyle();
}

void ObserverEntity::output(std::ostream &os) const {
  os << "Observer{id: " << id
      << ", position: " << position
      << ", startTime: " << _startTime
      << ", totalLifetime: " << _totalLifetime
      << ", lifeFraction: " << _lifeFraction
      << "}";
}
