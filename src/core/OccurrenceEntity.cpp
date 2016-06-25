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
: ::Params("Occurrences")
, radius("Radius Range")
, spawnArea("Spawn Area") {
  add(radius
      .set(0.4, 1.3)
      .setParamRange(0, 4));
  add(spawnArea
      .set(ofVec3f(-1), ofVec3f(1))
      .setParamRange(ofVec3f(-2), ofVec3f(2)));
  add(markerColor.set("Marker Color", ofFloatColor(1, .5, .25, 1)));
  add(rangeColor.set("Range Color", ofFloatColor(.5, .5, .5, 0.2)));
  add(connectorColor.set("Connector Color", ofFloatColor(.5, .5, .5, 1.0)));
}

void OccurrenceEntity::Params::initPanel(ofxGuiGroup &panel) {
  panel.getGroup("Spawn Area").minimize();
  panel.getGroup("Marker Color").minimize();
  panel.getGroup("Range Color").minimize();
  panel.getGroup("Connector Color").minimize();
}

shared_ptr<OccurrenceEntity> OccurrenceEntity::spawn(const OccurrenceEntity::Params &params) {
  ofVec3f pos = params.spawnArea.getValue();
  float radius = params.radius.getValue();
  return shared_ptr<OccurrenceEntity>(new OccurrenceEntity(pos, radius, params));
}

OccurrenceEntity::OccurrenceEntity(ofVec3f pos, float radius, const Params& params)
: WorldObject()
, _actualRadius(0)
, _originalRadius(radius)
, _params(params) {
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
  float amount = getAmountOfObservation(state);
  
  float alpha = ofMap(amount, 0, MAX_OBS_LEVEL, 0.02f, 0.2f, true);
  
  
  ofPushStyle();
  ofFloatColor markerColor = _params.markerColor.get();
  markerColor.a *= alpha;
  ofSetColor(markerColor);
  ofDrawBox(position, 0.02);
  ofPopStyle();

  ofPushStyle();
  ofFloatColor rangeColor = _params.rangeColor.get();
  rangeColor *= alpha;
  ofSetColor(rangeColor);
  ofDrawSphere(position, _actualRadius);
  ofPopStyle();
  
  ofPushStyle();
  ofFloatColor connectorColor = _params.connectorColor.get();
  ofMesh connectorMesh;
  for (auto observer : _connectedObservers) {
    connectorMesh.addVertex(position);
    connectorMesh.addColor(ofFloatColor(connectorColor, connectorColor.a * alpha));
    connectorMesh.addVertex(observer.second->position);
    connectorMesh.addColor(ofFloatColor(connectorColor, connectorColor.a * observer.second->getRemainingLifetimeFraction(state)));
  }
  connectorMesh.setMode(OF_PRIMITIVE_LINES);
  connectorMesh.draw();
  ofPopStyle();
}

void OccurrenceEntity::output(std::ostream &os) const {
  os << "OccurrenceEntity{id: " << id
      << ", position: " << position
      << ", originalRadius: " << _originalRadius
      << ", actualRadius: " << _actualRadius
      << "}";
}
