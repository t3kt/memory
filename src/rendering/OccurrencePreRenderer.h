//
//  OccurrencePreRenderer.h
//  memory
//
//  Created by tekt on 8/7/16.
//
//

#ifndef OccurrencePreRenderer_h
#define OccurrencePreRenderer_h

#include "../rendering/EntityRenderer.h"

class OccurrenceEntity;

class OccurrencePreRendererParams
: public EntityPreRendererParams {
public:
  OccurrencePreRendererParams() {
    add(sizeRange
        .setKey("sizeRange")
        .setName("Draw Size Range")
        .setParamValuesAndDefaults(10, 25)
        .setParamRanges(0, 100));
    add(connectionCountRange
        .setKey("connectionCountRange")
        .setName("Connection Amount Scale")
        .setParamValuesAndDefaults(0, 4)
        .setParamRanges(0, 20));
  }

  ValueRange<float> sizeRange;
  ValueRange<float> connectionCountRange;
};

class OccurrencePreRenderer
: public EntityPreRenderer<OccurrenceEntity, OccurrencePreRendererParams> {
public:

  OccurrencePreRenderer(const Params& params,
                        const ColorTheme& colors,
                        Context& context);

  void update() override;
};

#endif /* OccurrencePreRenderer_h */
