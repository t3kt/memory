//
//  OccurrenceRenderer.h
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#ifndef OccurrenceRenderer_h
#define OccurrenceRenderer_h

#include "../rendering/EntityRenderer.h"

class MemoryAppParameters;

class OccurrenceRenderer
: public AbstractEntityRenderer {
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
    TParam<bool> wireEnabled;
    TParam<float> wireScale;
    TParam<float> wireSaturation;
    TParam<float> wireBrightness;
  };

  OccurrenceRenderer(const Params& params,
                     const MemoryAppParameters& appParams,
                     Context& context);

  void draw() override;
private:
  const Params& _params;
  const ofFloatColor& _rangeColor;
  const MemoryAppParameters& _appParams;
  ObjectManager<OccurrenceEntity>& _entities;
};

#endif /* OccurrenceRenderer_h */
