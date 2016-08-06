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

class ObserverOccurrenceConnectorRenderer {
public:
  class Params : public ParamsWithEnabled {
  public:
    Params() {
      add(connectionCountRange
          .setKey("connectionCountRange")
          .setName("Connection Count Range")
          .setParamValuesAndDefaults(0, 4)
          .setParamRanges(0, 20));
      setEnabledValueAndDefault(true);
    }

    ValueRange<float> connectionCountRange;
  };
  ObserverOccurrenceConnectorRenderer(const Params& params, const ofFloatColor& color, const ObjectManager<OccurrenceEntity>& occurrences)
  : _params(params)
  , _color(color)
  , _occurrences(occurrences) { }

  void draw(const State& state);

private:
  const Params& _params;
  const ofFloatColor& _color;
  const ObjectManager<OccurrenceEntity>& _occurrences;
};

class ObserverObserverConnectorRenderer {
public:
  using Params = ParamsWithEnabled;

  ObserverObserverConnectorRenderer(const Params& params,
                                    const ofFloatColor& color,
                                    const ObjectManager<ObserverEntity>& entities)
  : _params(params)
  , _color(color)
  , _entities(entities) { }

  void draw(const State& state);

private:
  const Params& _params;
  const ofFloatColor& _color;
  const ObjectManager<ObserverEntity>& _entities;
};

class OccurrenceOccurrenceConnectorRenderer {
public:
  using Params = ParamsWithEnabled;

  OccurrenceOccurrenceConnectorRenderer(const Params& params,
                                        const ofFloatColor& color,
                                        const ObjectManager<OccurrenceEntity>& entities)
  : _params(params)
  , _color(color)
  , _entities(entities) { }

  void draw(const State& state);

private:
  const Params& _params;
  const ofFloatColor& _color;
  const ObjectManager<OccurrenceEntity>& _entities;
};

#endif /* ConnectorRenderer_h */
