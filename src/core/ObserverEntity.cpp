//
//  ObserverEntity.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include <ofMain.h>

ObserverEntity::Params::Params() {
  paramGroup.add(lifetimeRange.set("Lifetime Range", ofVec2f(5000, 60000)));
}

ObserverEntity::ObserverEntity(ofVec3f pos, float life, const State& state)
: Entity()
, _startTime(state.time)
, _totalLifetime(life)
{
  position = pos;
}

void ObserverEntity::addOccurrence(shared_ptr<OccurrenceEntity> occurrence) {
  _connectedOccurrences.push_back(occurrence);
}

float ObserverEntity::getRemainingLifetimeFraction(const State &state) {
  float elapsed = state.time - _startTime;
  if (elapsed > _totalLifetime) {
    return 0.0f;
  }
  return ofMap(elapsed, 0.0f, _totalLifetime, 1.0f, 0.0f);
}

void ObserverEntity::draw(State &state) {
  float alpha = getRemainingLifetimeFraction(state);
  ofPushMatrix();
  
  ofTranslate(position);
  
  ofPushStyle();
  ofFill();
  ofSetColor(ofColor(ofColor::black, alpha));
  ofDrawCircle(0, 0, 20);
  ofPopStyle();
  
  ofPushStyle();
  ofNoFill();
  ofSetColor(ofColor(ofColor::black, ofClamp(alpha + 0.1f, 0, 1)));
  ofDrawCircle(0, 0, 20);
  ofPopStyle();
  
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
