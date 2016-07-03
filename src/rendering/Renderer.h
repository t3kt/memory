//
//  Renderer.h
//  memory
//
//  Created by tekt on 7/2/16.
//
//

#ifndef Renderer_h
#define Renderer_h

#include "Params.h"
#include "Colors.h"
#include "State.h"
#include "OccurrenceEntity.h"
#include "ObserverEntity.h"
#include "ObjectManager.h"

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

    Json to_json() const override;
    void read_json(const Json& obj) override;

    TParam<float> size;
  };

  AbstractEntityRenderer(const Params& params, const ofFloatColor& color)
  : _baseParams(params)
  , _color(color) { }

  virtual void draw(const State& state) = 0;
protected:
  const Params& _baseParams;
  const ofFloatColor& _color;
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
  virtual void drawEntity(const T& entity, const ofFloatColor& baseColor, float size, const State& state) const = 0;
};

class ObserverRenderer
: public EntityRenderer<ObserverEntity> {
public:

  ObserverRenderer(const Params& params,
                   const ColorTheme& colors,
                   ObjectManager<ObserverEntity>& entities)
  : EntityRenderer<ObserverEntity>(params, colors.observerMarker.get())
  , _entities(entities) { }
protected:
  ObjectManager<ObserverEntity>::StorageList::iterator begin() override {
    return _entities.begin();
  }
  ObjectManager<ObserverEntity>::StorageList::iterator end() override {
    return _entities.end();
  }
  void drawEntity(const ObserverEntity& entity, const ofFloatColor& baseColor, float size, const State& state) const override;
private:
  ObjectManager<ObserverEntity>& _entities;
};

class OccurrenceRenderer
: public EntityRenderer<OccurrenceEntity> {
public:
  class Params : public AbstractEntityRenderer::Params {
  public:
    Params();

    Json to_json() const override;
    void read_json(const Json& obj) override;

    ValueRange<float> connectionCountRange;
  };

  OccurrenceRenderer(const Params& params,
                     const ColorTheme& colors,
                     ObjectManager<OccurrenceEntity>& entities)
  : EntityRenderer<OccurrenceEntity>(params, colors.occurrenceMarker.get())
  , _params(params)
  , _entities(entities) { }
protected:
  ObjectManager<OccurrenceEntity>::StorageList::iterator begin() override {
    return _entities.begin();
  }
  ObjectManager<OccurrenceEntity>::StorageList::iterator end() override {
    return _entities.end();
  }
  void drawEntity(const OccurrenceEntity& entity, const ofFloatColor& baseColor, float size, const State& state) const override;
private:
  const Params& _params;
  ObjectManager<OccurrenceEntity>& _entities;
};

//class ObserverOccurrenceConnectorRenderer

#endif /* Renderer_h */
