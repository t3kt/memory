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

class OccurrenceRenderer {
public:
  class Params : public ParamsWithEnabled {
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
      add(highlightAmount
          .setKey("highlightAmount")
          .setName("Highlighting")
          .setValueAndDefault(0.6)
          .setRange(0, 1));
    }

    ValueRange<float> sizeRange;
    TParam<bool> showRange;
    TParam<bool> wireEnabled;
    TParam<float> wireScale;
    TParam<float> wireSaturation;
    TParam<float> wireBrightness;
    TParam<float> highlightAmount;
  };

  OccurrenceRenderer(const Params& params,
                     const ColorTheme& colors,
                     Context& context);

  void draw();
private:
  const Params& _params;
  Context& _context;
  const ColorTheme& _colors;
  ObjectManager<OccurrenceEntity>& _entities;
};

#endif /* OccurrenceRenderer_h */
