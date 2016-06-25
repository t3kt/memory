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

ObserverEntity::Params::Params() {
  paramGroup.add(lifetimeRange.set("Lifetime Range", ofVec2f(1, 4)));
}

ObserverEntity::ObserverEntity(ofVec3f pos, float life, const State& state)
: WorldObject()
, _startTime(state.time)
, _totalLifetime(life)
{
  position = pos;
}

void ObserverEntity::addOccurrence(shared_ptr<OccurrenceEntity> occurrence) {
  _connectedOccurrences.push_back(occurrence);
}

float ObserverEntity::getRemainingLifetimeFraction(const State &state) const {
  float elapsed = state.time - _startTime;
  if (elapsed > _totalLifetime) {
    return 0.0f;
  }
  return ofMap(elapsed, 0.0f, _totalLifetime, 1.0f, 0.0f);
}

void ObserverEntity::handleDeath() {
  for (auto occurrence : _connectedOccurrences) {
    occurrence->removeObserver(id);
  }
}

void ObserverEntity::draw(const State &state) {
  float alpha = getRemainingLifetimeFraction(state);
  if (alpha <= 0) {
    return;
  }
  ofPushMatrix();
  
  //ofTranslate(position);
  
  ofPushStyle();
    ofFill();
  ofSetColor(ofColor::blue, alpha * 255);
  //ofDrawCircle(0, 0, 20);
  ofDrawSphere(position, 0.03);
//  ofDrawCircle(position, 0.03);
  ofPopStyle();
  
//  ofPushStyle();
//  ofNoFill();
//  ofSetColor(ofColor(ofColor::black, ofClamp(alpha + 0.1f, 0, 1)));
//  ofDrawCircle(0, 0, 20);
//  ofPopStyle();
  
//  ofPath path;
//  path.setFilled(true);
//  path.setFillColor(ofColor(ofColor::black, alpha));
//  path.setStrokeWidth(1.0f);
//  path.setStrokeColor(ofColor(ofColor::black,
//                              ofClamp(alpha + 1.0f, 0.0f, 1.0f)));
//  path.circle(ofVec3f::zero(), 5);
//  path.draw();
  
  ofPopMatrix();
}

void ObserverEntity::output(std::ostream &os) const {
  os << "Observer{id: " << id
      << ", position: " << position
      << ", startTime: " << _startTime
      << ", totalLifetime: " << _totalLifetime
      << "}";
}
