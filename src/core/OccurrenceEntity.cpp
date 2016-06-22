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
  
  ofTranslate(position);
  
  float amount = getAmountOfObservation(state);
  
  float alpha = ofMap(amount, 0, MAX_OBS_LEVEL, 0.02f, 0.2f, true);
  
  
  ofPushStyle();
//  ofPath markerPath;
//  markerPath.setFillColor(ofColor(1.0f, 0.5f, 0.25f, alpha));
//  ofRectangle rect;
//  rect.setFromCenter(0, 0, 5, 5);
//  markerPath.rectangle(rect);
//  markerPath.draw();
  ofSetRectMode(OF_RECTMODE_CENTER);
  ofSetColor(ofColor(1.0f, 0.5f, 0.25f, alpha));
  ofFill();
  ofDrawRectangle(0, 0, 5, 5);
  ofPopStyle();

  ofPushStyle();
  ofSetColor(ofColor(0.5f, 0.5f, 0.5f, alpha));
  ofFill();
  ofDrawCircle(0, 0, _actualRadius);
  ofPopStyle();
  
  ofPushStyle();
  ofSetColor(ofColor(0.7f, 0.7f, 0.7f, ofClamp(alpha + 0.1f, 0, 1)));
  ofNoFill();
  ofDrawCircle(0, 0, _actualRadius);
  ofPopStyle();
  
  ofPopMatrix();
}
