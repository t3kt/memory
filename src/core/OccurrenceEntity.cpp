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

OccurrenceEntity::Params::Params() {
  paramGroup.add(radiusRange.set("Radius Range", ofVec2f(0.4, 1.3)));
}

OccurrenceEntity::OccurrenceEntity(ofVec3f pos, float radius)
: _actualRadius(0)
, _originalRadius(radius)
, WorldObject() {
  position = pos;
}

void OccurrenceEntity::addObserver(shared_ptr<ObserverEntity> observer) {
  _connectedObservers[observer->id] = observer;
  float dist = position.distance(observer->position);
  if (dist > _actualRadius) {
    _actualRadius = dist;
  }
}

void OccurrenceEntity::recalculateRadius() {
  _actualRadius = 0;
  for (auto observer : _connectedObservers) {
    float dist = position.distance(observer.second->position);
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

float OccurrenceEntity::getAmountOfObservation(const State& state) const {
  float result = 0;
  for (auto observer : _connectedObservers) {
    result += observer.second->getRemainingLifetimeFraction(state);
  }
  return result;
}

void OccurrenceEntity::draw(const State &state) {
//  ofPushMatrix();
  
//  ofTranslate(position);
  
  float amount = getAmountOfObservation(state);
  
  float alpha = ofMap(amount, 0, MAX_OBS_LEVEL, 0.02f, 0.2f, true);
  
  
  ofPushStyle();
//  ofPath markerPath;
//  markerPath.setFillColor(ofColor(1.0f, 0.5f, 0.25f, alpha));
//  ofRectangle rect;
//  rect.setFromCenter(0, 0, 5, 5);
//  markerPath.rectangle(rect);
//  markerPath.draw();
//  ofSetRectMode(OF_RECTMODE_CENTER);
//  ofSetColor(ofColor(1.0f, 0.5f, 0.25f, alpha));
  ofSetColor(255, 127, 63, alpha * 255);
//  ofFill();
//  ofDrawRectangle(0, 0, 5, 5);
  ofDrawBox(position, 0.02);
  ofPopStyle();

  ofPushStyle();
  //  ofSetColor(ofColor(0.5f, 0.5f, 0.5f, alpha));
  ofSetColor(127, 127, 127, alpha * 255);
//  ofFill();
//  ofDrawCircle(0, 0, _actualRadius);
  ofDrawSphere(position, _actualRadius);
  ofPopStyle();
  
//  ofPushStyle();
//  ofSetColor(ofColor(0.7f, 0.7f, 0.7f, ofClamp(alpha + 0.1f, 0, 1)));
//  ofNoFill();
//  ofDrawCircle(0, 0, _actualRadius);
//  ofPopStyle();
  
//  ofPushStyle();
//  ofFill();
//  ofSetColor(ofColor::red);
//  ofDrawCircle(0, 0, 5);
//  ofPopStyle();
  
  ofPushStyle();
  ofSetColor(190, 190, 190);
  ofMesh connectorMesh;
  for (auto observer : _connectedObservers) {
    connectorMesh.addVertex(position);
    connectorMesh.addColor(ofColor(127, 127, 127, alpha * 255));
    connectorMesh.addVertex(observer.second->position);
    connectorMesh.addColor(ofColor(127, 127, 127, observer.second->getRemainingLifetimeFraction(state)*255));
//    ofDrawLine(position, observer->position);
  }
  connectorMesh.setMode(OF_PRIMITIVE_LINES);
  connectorMesh.draw();
  ofPopStyle();
  
//  ofPopMatrix();
}

void OccurrenceEntity::output(std::ostream &os) const {
  os << "OccurrenceEntity{id: " << id
      << ", position: " << position
      << ", originalRadius: " << _originalRadius
      << ", actualRadius: " << _actualRadius
      << "}";
}
