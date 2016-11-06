//
//  ConnectorRenderer.h
//  memory
//
//  Created by tekt on 7/18/16.
//
//

#pragma once

#include <ofGraphics.h>
#include <ofTypes.h>
#include <ofVboMesh.h>
#include "../control/Params.h"
#include "../core/ObjectManager.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"
#include "../rendering/Renderer.h"

class ConnectorRendererParams
: public ParamsWithEnabled {
public:
  ConnectorRendererParams() {
    add(lineWidth
        .setKey("lineWidth")
        .setName("Line Width")
        .setRange(0, 30)
        .setValueAndDefault(2));
  }

  TParam<float> lineWidth;
};

class ConnectionRenderer
: public Renderer {
public:
  using Params = ConnectorRendererParams;

  ConnectionRenderer(const Params& params,
                     const ofFloatColor& color);

protected:
  void clearMesh();

  void addConnector(const AbstractConnection& connection,
                    const ofFloatColor& startColor,
                    const ofVec3f& startPos);

  template<typename TConn>
  void addConnectors(const ParticleObject& sourceEntity,
                     const TypedEntityConnectionMap<TConn>& connections) {
//    if (!sourceEntity.visible()) {
//      return;
//    }
    auto startColor = ofFloatColor(_color,
                                   _color.a * sourceEntity.alpha());
    const auto& startPos = sourceEntity.position();
    for (const auto& connection : connections) {
      addConnector(*connection,
                   startColor,
                   startPos);
    }
  }

  void drawMesh();

  const Params& _params;
  const ofFloatColor& _color;
  ofMesh _mesh;
};

template<typename S, typename T>
class TypedConnectionRenderer
: public ConnectionRenderer {
public:
  using ConnT = EntityConnection<T>;
  using ConnMapT = TypedEntityConnectionMap<ConnT>;

  TypedConnectionRenderer(const Params& params,
                          const ofFloatColor& color,
                          const ObjectManager<S>& entities)
  : ConnectionRenderer(params, color)
  , _entities(entities) { }

  void draw() override {
    if (!_params.enabled.get()) {
      return;
    }
    clearMesh();
    for (const auto& entity : _entities) {
      addConnectors(*entity,
                    getEntityConnections(*entity));
    }
    drawMesh();
  }

private:
  const ConnMapT& getEntityConnections(const S& entity) const;

  const ObjectManager<S>& _entities;
};

using ObserverOccurrenceConnectorRenderer =
TypedConnectionRenderer<ObserverEntity, OccurrenceEntity>;

using ObserverObserverConnectorRenderer =
TypedConnectionRenderer<ObserverEntity, ObserverEntity>;

using OccurrenceOccurrenceConnectorRenderer =
TypedConnectionRenderer<OccurrenceEntity, OccurrenceEntity>;

