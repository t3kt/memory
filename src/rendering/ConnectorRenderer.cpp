//
//  ConnectorRenderer.cpp
//  memory
//
//  Created by tekt on 7/18/16.
//
//

#include <ofMain.h>
#include "../rendering/ConnectorRenderer.h"

ConnectionRenderer::ConnectionRenderer(const Params& params,
                                       const ofFloatColor& color)
: _params(params)
, _color(color) {
  _mesh.setMode(OF_PRIMITIVE_LINES);
}

void ConnectionRenderer::clearMesh() {
  _mesh.clear();
}

void ConnectionRenderer
::addConnector(const AbstractConnection &connection,
               const ofFloatColor &startColor) {
  if (!connection.visible()) {
    return;
  }
  _mesh.addVertex(connection.sourcePosition());
  _mesh.addColor(startColor);
  _mesh.addVertex(connection.endPosition());
  _mesh.addColor(ofFloatColor(_color,
                              _color.a * connection.targetEntity().alpha()));
}

void ConnectionRenderer::drawMesh() {
  auto renderer = ofGetCurrentRenderer();
  renderer->pushStyle();
  renderer->setBlendMode(OF_BLENDMODE_ALPHA);
//  renderer->setLineWidth(_params.lineWidth.get());

  renderer->draw(_mesh,
                 OF_MESH_WIREFRAME,
                 _mesh.usingColors(),
                 _mesh.usingTextures(),
                 _mesh.usingNormals());

  renderer->popStyle();
}

void ObserverOccurrenceConnectorRenderer::draw() {
  if (!_params.enabled()) {
    return;
  }
  clearMesh();
  for (const auto& entity : _occurrences) {
    addConnectors(*entity,
                  entity->getObserverConnections());
  }
  drawMesh();
}

void ObserverObserverConnectorRenderer::draw() {
  if (!_params.enabled()) {
    return;
  }
  clearMesh();
  for (const auto& entity : _entities) {
    addConnectors(*entity,
                  entity->getObserverConnections());
  }
  drawMesh();
}

void OccurrenceOccurrenceConnectorRenderer::draw() {
  if (!_params.enabled()) {
    return;
  }
  clearMesh();
  for (const auto& entity : _entities) {
    addConnectors(*entity,
                  entity->getOccurrenceConnections());
  }
  drawMesh();
}
