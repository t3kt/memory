//
//  ConnectorRenderer.cpp
//  memory
//
//  Created by tekt on 7/18/16.
//
//

#include <ofMain.h>
#include "ConnectorRenderer.h"

void ObserverOccurrenceConnectorRenderer::draw() {
  if (!_params.enabled()) {
    return;
  }
  ofPushStyle();
  ofEnableAlphaBlending();
  ofSetLineWidth(_params.lineWidth.get());
  ofMesh connectorMesh;
  connectorMesh.setMode(OF_PRIMITIVE_LINES);
  for (const auto& occurrence : _occurrences) {
    if (!occurrence->visible()) {
      continue;
    }
    ofFloatColor connectorStartColor(_color,
                                     _color.a * occurrence->alpha());
    for (const auto& observer : occurrence->connectedObservers()) {
      if (!observer.second->visible()) {
        continue;
      }
      connectorMesh.addVertex(occurrence->position());
      connectorMesh.addColor(connectorStartColor);
      connectorMesh.addVertex(observer.second->position());
      connectorMesh.addColor(ofFloatColor(_color,
                                          _color.a * observer.second->alpha()));
    }
  }
  connectorMesh.draw();
  ofPopStyle();
}

void ObserverObserverConnectorRenderer::draw() {
  if (!_params.enabled()) {
    return;
  }
  ofPushStyle();
  ofEnableAlphaBlending();
  ofSetLineWidth(_params.lineWidth.get());
  ofMesh connectorMesh;
  connectorMesh.setMode(OF_PRIMITIVE_LINES);
  for (const auto& observer : _entities) {
    if (!observer->visible()) {
      continue;
    }
    ofFloatColor connectorStartColor(_color, _color.a * observer->alpha());
    for (const auto& other : observer->getConnectedObservers()) {
      if (!other.second->visible()) {
        continue;
      }
      connectorMesh.addVertex(observer->position());
      connectorMesh.addColor(connectorStartColor);
      connectorMesh.addVertex(other.second->position());
      connectorMesh.addColor(ofFloatColor(_color,
                                          _color.a * other.second->alpha()));
    }
  }
  connectorMesh.draw();
  ofPopStyle();
}

void OccurrenceOccurrenceConnectorRenderer::draw() {
  if (!_params.enabled()) {
    return;
  }
  ofPushStyle();
  ofEnableAlphaBlending();
  ofSetLineWidth(_params.lineWidth.get());
  ofMesh connectorMesh;
  connectorMesh.setMode(OF_PRIMITIVE_LINES);
  for (const auto& occurrence : _entities) {
    if (!occurrence->visible()) {
      continue;
    }
    ofFloatColor connectorStartColor(_color,
                                     _color.a * occurrence->alpha());
    for (const auto& other : occurrence->getConnectedOccurrences()) {
      if (!other.second->visible()) {
        continue;
      }
      connectorMesh.addVertex(occurrence->position());
      connectorMesh.addColor(connectorStartColor);
      connectorMesh.addVertex(other.second->position());
      connectorMesh.addColor(ofFloatColor(_color,
                                          _color.a * other.second->alpha()));
    }
  }
  connectorMesh.draw();
  ofPopStyle();
}
