//
//  ConnectorRenderer.cpp
//  memory
//
//  Created by tekt on 7/18/16.
//
//

#include <ofMain.h>
#include "../rendering/ConnectorRenderer.h"

void ConnectionRenderer
::addConnector(ofMesh& mesh,
               const AbstractConnection &connection,
               const ofFloatColor &startColor) {
  if (!connection.visible()) {
    return;
  }
  mesh.addVertex(connection.sourcePosition());
  mesh.addColor(startColor);
  mesh.addVertex(connection.endPosition());
  mesh.addColor(ofFloatColor(_color,
                             _color.a * connection.targetEntity().alpha()));
}

void ConnectionRenderer::drawMesh(const ofMesh &mesh) {
  auto renderer = ofGetCurrentRenderer();
  renderer->pushStyle();
  renderer->setBlendMode(OF_BLENDMODE_ALPHA);
//  renderer->setLineWidth(_params.lineWidth.get());

  renderer->draw(mesh,
                 OF_MESH_WIREFRAME,
                 mesh.usingColors(),
                 mesh.usingTextures(),
                 mesh.usingNormals());

  renderer->popStyle();
}

void ObserverOccurrenceConnectorRenderer::draw() {
  if (!_params.enabled()) {
    return;
  }
  ofMesh mesh;
  mesh.setMode(OF_PRIMITIVE_LINES);
  for (const auto& entity : _occurrences) {
    addConnectors(mesh,
                  *entity,
                  entity->getObserverConnections());
  }
  drawMesh(mesh);
}

void ObserverObserverConnectorRenderer::draw() {
  if (!_params.enabled()) {
    return;
  }
  ofMesh mesh;
  mesh.setMode(OF_PRIMITIVE_LINES);
  for (const auto& entity : _entities) {
    addConnectors(mesh,
                  *entity,
                  entity->getObserverConnections());
  }
  drawMesh(mesh);
}

void OccurrenceOccurrenceConnectorRenderer::draw() {
  if (!_params.enabled()) {
    return;
  }
  ofMesh mesh;
  mesh.setMode(OF_PRIMITIVE_LINES);
  for (const auto& entity : _entities) {
    addConnectors(mesh,
                  *entity,
                  entity->getOccurrenceConnections());
  }
  drawMesh(mesh);
}
