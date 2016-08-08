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
#include "ObjectManager.h"
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include "Params.h"

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

class ObserverOccurrenceConnectorRenderer {
public:
  using Params = ConnectorRendererParams;

  ObserverOccurrenceConnectorRenderer(const Params& params, const ofFloatColor& color, const ObjectManager<OccurrenceEntity>& occurrences)
  : _params(params)
  , _color(color)
  , _occurrences(occurrences) { }

  void draw();

private:
  const Params& _params;
  const ofFloatColor& _color;
  const ObjectManager<OccurrenceEntity>& _occurrences;
};

class ObserverObserverConnectorRenderer {
public:
  using Params = ConnectorRendererParams;

  ObserverObserverConnectorRenderer(const Params& params,
                                    const ofFloatColor& color,
                                    const ObjectManager<ObserverEntity>& entities)
  : _params(params)
  , _color(color)
  , _entities(entities) { }

  void draw();

private:
  const Params& _params;
  const ofFloatColor& _color;
  const ObjectManager<ObserverEntity>& _entities;
};

class OccurrenceOccurrenceConnectorRenderer {
public:
  using Params = ConnectorRendererParams;

  OccurrenceOccurrenceConnectorRenderer(const Params& params,
                                        const ofFloatColor& color,
                                        const ObjectManager<OccurrenceEntity>& entities)
  : _params(params)
  , _color(color)
  , _entities(entities) { }

  void draw();

private:
  const Params& _params;
  const ofFloatColor& _color;
  const ObjectManager<OccurrenceEntity>& _entities;
};

#endif /* ConnectorRenderer_h */
