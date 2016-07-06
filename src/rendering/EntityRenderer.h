//
//  EntityRenderer.h
//  memory
//
//  Created by tekt on 7/2/16.
//
//

#ifndef EntityRenderer_h
#define EntityRenderer_h

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

enum class EntityShape {
  SPHERE,
  BOX,
};

extern EnumTypeInfo<EntityShape> EntityShapeType;

class AbstractEntityRenderer {
public:
  class Params : public ::Params {
  public:
    Params();

    float size() const { return _size.get(); }
    void setSize(float size) { _size.set(size); }

    RampFactory<float>::Params fadeIn;

  protected:
    TParam<float> _size;
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

  void draw(const State& state) override;
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
  ofSpherePrimitive _markerMesh;
};

class OccurrenceRenderer
: public EntityRenderer<OccurrenceEntity> {
public:
  class Params : public AbstractEntityRenderer::Params {
  public:
    Params();

    TParam<bool> showRange;
    ValueRange<float> connectionCountRange;
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
  class Params : public ::Params {
  public:
    Params();

    bool enabled() const { return _enabled.get(); }

    ValueRange<float> connectionCountRange;
  private:
    TParam<bool> _enabled;
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

#endif /* EntityRenderer_h */
