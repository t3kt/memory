//
//  ObserverPreRenderer.h
//  memory
//
//  Created by tekt on 8/7/16.
//
//

#ifndef ObserverPreRenderer_h
#define ObserverPreRenderer_h

#include "../rendering/EntityRenderer.h"

class ObserverEntity;

class ObserverPreRendererParams
: public EntityPreRendererParams {
public:
  ObserverPreRendererParams() {
    add(size
        .setKey("size")
        .setName("Draw Size")
        .setValueAndDefault(2)
        .setRange(0, 20));
  }

  TParam<float> size;
};

class ObserverPreRenderer
: public EntityPreRenderer<ObserverEntity, ObserverPreRendererParams> {
public:

  ObserverPreRenderer(const Params& params,
                      const ColorTheme& colors,
                      Context& context);

  void update() override;
};

#endif /* ObserverPreRenderer_h */
