//
//  EntityRenderer.h
//  memory
//
//  Created by tekt on 7/2/16.
//
//

#ifndef EntityRenderer_h
#define EntityRenderer_h

#include <ofGraphics.h>
#include <ofTypes.h>
#include <ofVboMesh.h>
#include <of3dPrimitives.h>
#include <ofxChoreograph.h>

#include "Params.h"
#include "Colors.h"
#include "State.h"
#include "OccurrenceEntity.h"
#include "ObserverEntity.h"
#include "ObjectManager.h"
#include "AnimationObject.h"
#include "NavigatorEntity.h"

enum class EntityShape {
  SPHERE,
  BOX,
};

extern EnumTypeInfo<EntityShape> EntityShapeType;

class AbstractEntityRenderer {
public:
  class Params : public ::Params {
  public:
    Params() {
      add(size
          .setKey("size")
          .setName("Draw Size")
          .setValueAndDefault(10)
          .setRange(0, 100));
      add(fadeIn
          .setKey("fadeIn")
          .setName("Fade In"));
    }

    RampFactory<float>::Params fadeIn;
    TParam<float> size;
  };

  AbstractEntityRenderer(const Params& params, const ofFloatColor& color)
  : _baseParams(params)
  , _color(color)
  , _fadeIn(params.fadeIn) { }

  virtual void update(const State& state);
  virtual void draw(const State& state) = 0;
protected:
  const Params& _baseParams;
  const ofFloatColor& _color;
  RampFactory<float> _fadeIn;
};

template<typename T>
class EntityRenderer
: public AbstractEntityRenderer {
public:
  EntityRenderer(const Params& params,
                 const ofFloatColor& color)
  : AbstractEntityRenderer(params, color) { }

  void draw(const State& state) override {
    ofFloatColor baseColor(_color);
    float size = _baseParams.size();
    ofPushStyle();
    ofFill();
    for (std::shared_ptr<T> entity : *this) {
      if (!entity->visible()) {
        continue;
      }
      drawEntity(*entity, baseColor, size, state);
    }
    ofPopStyle();
  }
protected:
  virtual typename ObjectManager<T>::StorageList::iterator begin() = 0;
  virtual typename ObjectManager<T>::StorageList::iterator end() = 0;
  virtual void drawEntity(const T& entity, const ofFloatColor& baseColor, float size, const State& state) = 0;
};

class ObserverRenderer
: public EntityRenderer<ObserverEntity> {
public:

  ObserverRenderer(const Params& params,
                   const ColorTheme& colors,
                   ObjectManager<ObserverEntity>& entities);
protected:
  ObjectManager<ObserverEntity>::StorageList::iterator begin() override {
    return _entities.begin();
  }
  ObjectManager<ObserverEntity>::StorageList::iterator end() override {
    return _entities.end();
  }
  void drawEntity(const ObserverEntity& entity, const ofFloatColor& baseColor, float size, const State& state) override;
private:
  ObjectManager<ObserverEntity>& _entities;
};

//class NavigatorRenderer
//: public EntityRenderer<NavigatorEntity> {
//
//}

class OccurrenceRenderer
: public EntityRenderer<OccurrenceEntity> {
public:
  class Params : public AbstractEntityRenderer::Params {
  public:
    Params() {
      add(showRange
          .setKey("showRange")
          .setName("Show Range")
          .setValueAndDefault(true));
      add(connectionCountRange
          .setKey("connectionCountRange")
          .setName("Connection Amount Scale")
          .setParamValuesAndDefaults(0, 4)
          .setParamRanges(0, 20));
      add(wireEnabled
          .setKey("wireEnabled")
          .setName("Wireframe Enabled")
          .setValueAndDefault(false));
      add(wireScale
          .setKey("wireScale")
          .setName("Wire Scale")
          .setValueAndDefault(1.2)
          .setRange(0.9, 1.8));
      add(wireSaturation
          .setKey("wireSaturation")
          .setName("Wire Saturation")
          .setValueAndDefault(0.5)
          .setRange(0, 1.5));
      add(wireBrightness
          .setKey("wireBrightness")
          .setName("Wire Brightness")
          .setValueAndDefault(1.1)
          .setRange(0.5, 1.5));
      size.setValueAndDefault(20);
    }

    TParam<bool> showRange;
    ValueRange<float> connectionCountRange;
    TParam<bool> wireEnabled;
    TParam<float> wireScale;
    TParam<float> wireSaturation;
    TParam<float> wireBrightness;
  };

  OccurrenceRenderer(const Params& params,
                     const ColorTheme& colors,
                     ObjectManager<OccurrenceEntity>& entities)
  : EntityRenderer<OccurrenceEntity>(params, colors.getColor(ColorId::OCCURRENCE_MARKER))
  , _params(params)
  , _rangeColor(colors.getColor(ColorId::OCCURRENCE_RANGE))
  , _entities(entities)
  , _markerMesh(1, 1, 1, 1, 1, 1) { }
protected:
  ObjectManager<OccurrenceEntity>::StorageList::iterator begin() override {
    return _entities.begin();
  }
  ObjectManager<OccurrenceEntity>::StorageList::iterator end() override {
    return _entities.end();
  }
  void drawEntity(const OccurrenceEntity& entity, const ofFloatColor& baseColor, float size, const State& state) override;
private:
  const Params& _params;
  const ofFloatColor& _rangeColor;
  ObjectManager<OccurrenceEntity>& _entities;
  ofBoxPrimitive _markerMesh;
};

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

#endif /* EntityRenderer_h */
