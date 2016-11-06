//
//  ConnectorRenderer.h
//  memory
//
//  Created by tekt on 7/18/16.
//
//

#ifndef ConnectorRenderer_h
#define ConnectorRenderer_h

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

class ObserverOccurrenceConnectorRenderer
: public ConnectionRenderer {
public:
  using Params = ConnectorRendererParams;

  ObserverOccurrenceConnectorRenderer(const Params& params,
                                      const ofFloatColor& color,
                                      const ObjectManager<OccurrenceEntity>& occurrences)
  : ConnectionRenderer(params, color)
  , _occurrences(occurrences) { }

  void draw() override;

private:
  const ObjectManager<OccurrenceEntity>& _occurrences;
};

class ObserverObserverConnectorRenderer
: public ConnectionRenderer {
public:
  using Params = ConnectorRendererParams;

  ObserverObserverConnectorRenderer(const Params& params,
                                    const ofFloatColor& color,
                                    const ObjectManager<ObserverEntity>& entities)
  : ConnectionRenderer(params, color)
  , _entities(entities) { }

  void draw() override;

private:
  const ObjectManager<ObserverEntity>& _entities;
};

class OccurrenceOccurrenceConnectorRenderer
: public ConnectionRenderer {
public:
  using Params = ConnectorRendererParams;

  OccurrenceOccurrenceConnectorRenderer(const Params& params,
                                        const ofFloatColor& color,
                                        const ObjectManager<OccurrenceEntity>& entities)
  : ConnectionRenderer(params, color)
  , _entities(entities) { }

  void draw() override;

private:
  const ObjectManager<OccurrenceEntity>& _entities;
};

#endif /* ConnectorRenderer_h */
