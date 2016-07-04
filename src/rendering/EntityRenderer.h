//
//  EntityRenderer.h
//  memory
//
//  Created by tekt on 7/2/16.
//
//

#ifndef EntityRenderer_h
#define EntityRenderer_h

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
    virtual void resetToDefaults() override {
      size.resetToDefault();
    }
    virtual bool hasDefaults() const override { return true; }

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

    virtual void resetToDefaults() override {
      showRange.resetToDefault();
      AbstractEntityRenderer::Params::resetToDefaults();
      connectionCountRange.resetToDefaults();
    }

    TParam<bool> showRange;
    ValueRange<float> connectionCountRange;
  };

  OccurrenceRenderer(const Params& params,
                     const ColorTheme& colors,
                     ObjectManager<OccurrenceEntity>& entities)
  : EntityRenderer<OccurrenceEntity>(params, colors.occurrenceMarker.get())
  , _params(params)
  , _rangeColor(colors.occurrenceRange.get())
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
  const ofFloatColor& _rangeColor;
  ObjectManager<OccurrenceEntity>& _entities;
};

class ObserverOccurrenceConnectorRenderer {
public:
  class Params : public ::Params {
  public:
    Params();

    Json to_json() const override;
    void read_json(const Json& obj) override;
    void resetToDefaults() override {
      enabled.resetToDefault();
      connectionCountRange.resetToDefaults();
    }
    bool hasDefaults() const override { return true; }

    TParam<bool> enabled;
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

#endif /* EntityRenderer_h */
