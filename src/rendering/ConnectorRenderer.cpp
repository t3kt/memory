//
//  ConnectorRenderer.cpp
//  memory
//
//  Created by tekt on 7/18/16.
//
//

#include <ofMain.h>
#include "../rendering/ConnectorRenderer.h"

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
    for (const auto& observer : occurrence->getConnectedObservers()) {
      if (!observer->visible()) {
        continue;
      }
      connectorMesh.addVertex(occurrence->position());
      connectorMesh.addColor(connectorStartColor);
      connectorMesh.addVertex(observer->position());
      connectorMesh.addColor(ofFloatColor(_color,
                                          _color.a * observer->alpha()));
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
    for (const auto& connection : observer->getObserverConnections()) {
      const auto& other = connection->entity();
      if (!other->visible()) {
        continue;
      }
      connectorMesh.addVertex(observer->position());
      connectorMesh.addColor(connectorStartColor);
      connectorMesh.addVertex(other->position());
      connectorMesh.addColor(ofFloatColor(_color,
                                          _color.a * other->alpha()));
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
      if (!other->visible()) {
        continue;
      }
      connectorMesh.addVertex(occurrence->position());
      connectorMesh.addColor(connectorStartColor);
      connectorMesh.addVertex(other->position());
      connectorMesh.addColor(ofFloatColor(_color,
                                          _color.a * other->alpha()));
    }
  }
  connectorMesh.draw();
  ofPopStyle();
}
