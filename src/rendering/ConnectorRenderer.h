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
#include "../core/ObjectManager.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"
#include "../core/Params.h"
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

class ObserverOccurrenceConnectorRenderer
: public Renderer {
public:
  using Params = ConnectorRendererParams;

  ObserverOccurrenceConnectorRenderer(const Params& params, const ofFloatColor& color, const ObjectManager<OccurrenceEntity>& occurrences)
  : _params(params)
  , _color(color)
  , _occurrences(occurrences) { }

  void draw() override;

private:
  const Params& _params;
  const ofFloatColor& _color;
  const ObjectManager<OccurrenceEntity>& _occurrences;
};

class ObserverObserverConnectorRenderer
: public Renderer {
public:
  using Params = ConnectorRendererParams;

  ObserverObserverConnectorRenderer(const Params& params,
                                    const ofFloatColor& color,
                                    const ObjectManager<ObserverEntity>& entities)
  : _params(params)
  , _color(color)
  , _entities(entities) { }

  void draw() override;

private:
  const Params& _params;
  const ofFloatColor& _color;
  const ObjectManager<ObserverEntity>& _entities;
};

class OccurrenceOccurrenceConnectorRenderer
: public Renderer {
public:
  using Params = ConnectorRendererParams;

  OccurrenceOccurrenceConnectorRenderer(const Params& params,
                                        const ofFloatColor& color,
                                        const ObjectManager<OccurrenceEntity>& entities)
  : _params(params)
  , _color(color)
  , _entities(entities) { }

  void draw() override;

private:
  const Params& _params;
  const ofFloatColor& _color;
  const ObjectManager<OccurrenceEntity>& _entities;
};

#endif /* ConnectorRenderer_h */
