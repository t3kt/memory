//
//  OccurrenceRenderer.h
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#ifndef OccurrenceRenderer_h
#define OccurrenceRenderer_h

#include "EntityRenderer.h"

class MemoryAppParameters;

class OccurrenceRenderer
: public EntityRenderer<OccurrenceEntity> {
public:
  class Params : public AbstractEntityRenderer::Params {
  public:
    Params() {
      add(sizeRange
          .setKey("sizeRange")
          .setName("Draw Size Range")
          .setParamValuesAndDefaults(10, 25)
          .setParamRanges(0, 100));
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
    }

    ValueRange<float> sizeRange;
    TParam<bool> showRange;
    ValueRange<float> connectionCountRange;
    TParam<bool> wireEnabled;
    TParam<float> wireScale;
    TParam<float> wireSaturation;
    TParam<float> wireBrightness;
  };

  OccurrenceRenderer(const Params& params,
                     const ColorTheme& colors,
                     const MemoryAppParameters& appParams,
                     ObjectManager<OccurrenceEntity>& entities)
  : EntityRenderer<OccurrenceEntity>(params, colors.getColor(ColorId::OCCURRENCE_MARKER))
  , _params(params)
  , _rangeColor(colors.getColor(ColorId::OCCURRENCE_RANGE))
  , _appParams(appParams)
  , _entities(entities) { }
protected:
  ObjectManager<OccurrenceEntity>::StorageList::iterator begin() override {
    return _entities.begin();
  }
  ObjectManager<OccurrenceEntity>::StorageList::iterator end() override {
    return _entities.end();
  }
  void drawEntity(const OccurrenceEntity& entity, const State& state) override;
private:
  const Params& _params;
  const ofFloatColor& _rangeColor;
  const MemoryAppParameters& _appParams;
  ObjectManager<OccurrenceEntity>& _entities;
};

#endif /* OccurrenceRenderer_h */
