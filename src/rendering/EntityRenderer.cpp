//
//  EntityRenderer.cpp
//  memory
//
//  Created by tekt on 7/2/16.
//
//

#include "AppAssets.h"
#include "EntityRenderer.h"
#include <ofMain.h>

using namespace ofxChoreograph;

void AbstractEntityRenderer::update(const State &state) {
  _fadeIn.update(state);
}

void ObserverOccurrenceConnectorRenderer::draw(const State& state) {
  if (!_params.enabled()) {
    return;
  }
  ofPushStyle();
  ofEnableAlphaBlending();
  ofMesh connectorMesh;
  float lowCount = _params.connectionCountRange.lowValue();
  float highCount = _params.connectionCountRange.highValue();
  connectorMesh.setMode(OF_PRIMITIVE_LINES);
  for (const auto& occurrence : _occurrences) {
    float occurrenceLife = occurrence->getAmountOfObservation();
    float occurrenceAlpha = ofMap(occurrenceLife,
                                  lowCount,
                                  highCount,
                                  0, 1, true);
    if (occurrenceAlpha <= 0) {
      continue;
    }
    ofFloatColor connectorStartColor(_color, _color.a * occurrenceAlpha);
    for (const auto& entry : occurrence->connectedObservers()) {
      auto observer = entry.second;
      float observerLife = observer->getRemainingLifetimeFraction();
      if (observerLife <= 0) {
        continue;
      }
      connectorMesh.addVertex(occurrence->position());
      connectorMesh.addColor(connectorStartColor);
      connectorMesh.addVertex(observer->position());
      connectorMesh.addColor(ofFloatColor(_color, _color.a * observerLife));
    }
  }
  connectorMesh.draw();
  ofPopStyle();
}

void ObserverObserverConnectorRenderer::draw(const State& state) {
  if (!_params.enabled()) {
    return;
  }
  ofPushStyle();
  ofEnableAlphaBlending();
  ofMesh connectorMesh;
  connectorMesh.setMode(OF_PRIMITIVE_LINES);
  for (const auto& observer : _entities) {
    if (!observer->alive()) {
      continue;
    }
    float observerLife = observer->getRemainingLifetimeFraction();
    ofFloatColor connectorStartColor(_color, _color.a * observerLife);
    for (const auto& other : observer->getConnectedObservers()) {
      if (!other.second->alive()) {
        continue;
      }
      float otherLife = other.second->getRemainingLifetimeFraction();
      connectorMesh.addVertex(observer->position());
      connectorMesh.addColor(connectorStartColor);
      connectorMesh.addVertex(other.second->position());
      connectorMesh.addColor(ofFloatColor(_color,
                                          _color.a * otherLife));
    }
  }
  connectorMesh.draw();
  ofPopStyle();
}

void OccurrenceOccurrenceConnectorRenderer::draw(const State& state) {
  if (!_params.enabled()) {
    return;
  }
  ofPushStyle();
  ofEnableAlphaBlending();
  ofMesh connectorMesh;
  connectorMesh.setMode(OF_PRIMITIVE_LINES);
  for (const auto& occurrence : _entities) {
    if (!occurrence->alive()) {
      continue;
    }
    float occurrenceLife = occurrence->getAmountOfObservation();
    ofFloatColor connectorStartColor(_color, _color.a * occurrenceLife);
    for (const auto& other : occurrence->getConnectedOccurrences()) {
      if (!other.second->alive()) {
        continue;
      }
      float otherLife = other.second->getAmountOfObservation();
      connectorMesh.addVertex(occurrence->position());
      connectorMesh.addColor(connectorStartColor);
      connectorMesh.addVertex(other.second->position());
      connectorMesh.addColor(ofFloatColor(_color,
                                          _color.a * otherLife));
    }
  }
  connectorMesh.draw();
  ofPopStyle();
}
