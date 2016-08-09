//
//  ObserverPreRenderer.h
//  memory
//
//  Created by tekt on 8/7/16.
//
//

#ifndef ObserverPreRenderer_h
#define ObserverPreRenderer_h

#include "EntityRenderer.h"

class ObserverEntity;

class ObserverPreRenderer
: public EntityPreRenderer<ObserverEntity> {
public:

  ObserverPreRenderer(const Params& params,
                      Context& context);

  void update() override;
};

#endif /* ObserverPreRenderer_h */