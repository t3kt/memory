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

class ObserverPreRenderer
: public EntityPreRenderer<ObserverEntity> {
public:

  ObserverPreRenderer(const Params& params,
                      const ColorTheme& colors,
                      Context& context);

  void update() override;
};

#endif /* ObserverPreRenderer_h */
