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

const float MAX_OBS_LEVEL = 4.0f;

OccurrenceEntity::OccurrenceEntity(ofVec3f pos, float radius)
: _actualRadius(0)
, _originalRadius(radius)
, Entity() {
  position = pos;
}

void OccurrenceEntity::addObserver(shared_ptr<ObserverEntity> observer) {
  _connectedObservers.push_back(observer);
  float dist = position.distance(observer->position);
  if (dist > _actualRadius) {
    _actualRadius = dist;
    drawRadius = dist;
  }
}

float OccurrenceEntity::getAmountOfObservation(const State& state) const {
  float result = 0;
  for (auto observer : _connectedObservers) {
    result += observer->getRemainingLifetimeFraction(state);
  }
  return result;
}

void OccurrenceEntity::draw(State &state) {
  ofPushMatrix();
  ofPushStyle();
  
  //....
  
  ofPopStyle();
  ofPopMatrix();
}
