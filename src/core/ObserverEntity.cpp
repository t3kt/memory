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

ObserverEntity::ObserverEntity(ofVec3f pos, float life, const State& state)
: Entity()
, _startTime(state.time)
, _totalLifetime(life)
{
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
  ofPushStyle();
  
  ofTranslate(position);
  
  ofPath path;
  path.setFilled(true);
  path.setFillColor(ofColor(ofColor::black, alpha));
  path.setStrokeWidth(1.0f);
  path.setStrokeColor(ofColor(ofColor::black,
                              ofClamp(alpha + 1.0f, 0.0f, 1.0f)));
  path.circle(ofVec3f::zero(), 5);
  path.draw();
  
  ofPopStyle();
  ofPopMatrix();
}
