//
//  ConnectorRenderer.cpp
//  memory
//
//  Created by tekt on 7/18/16.
//
//

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
               const ofFloatColor &startColor,
               const ofVec3f& startPos) {
//  if (!connection.visible()) {
//    return;
//  }
  _mesh.addVertex(startPos);
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

template<>
const TypedEntityConnectionMap<EntityConnection<OccurrenceEntity>>&
TypedConnectionRenderer<ObserverEntity, OccurrenceEntity>::getEntityConnections(const ObserverEntity& entity) const {
  return entity.getOccurrenceConnections();
}

template<>
const TypedEntityConnectionMap<EntityConnection<ObserverEntity>>&
TypedConnectionRenderer<ObserverEntity, ObserverEntity>::getEntityConnections(const ObserverEntity& entity) const {
  return entity.getObserverConnections();
}

template<>
const TypedEntityConnectionMap<EntityConnection<OccurrenceEntity>>&
TypedConnectionRenderer<OccurrenceEntity, OccurrenceEntity>::getEntityConnections(const OccurrenceEntity& entity) const {
  return entity.getOccurrenceConnections();
}
