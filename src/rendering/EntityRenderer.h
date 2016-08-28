//
//  EntityRenderer.h
//  memory
//
//  Created by tekt on 7/2/16.
//
//

#ifndef EntityRenderer_h
#define EntityRenderer_h

#include <ofGraphics.h>
#include <ofxChoreograph.h>
#include "../core/AnimationObject.h"
#include "../core/ObjectManager.h"
#include "../core/Params.h"

class Context;

class EntityPreRendererParams : public Params {
public:
  EntityPreRendererParams() {
    add(fadeIn
        .setKey("fadeIn")
        .setName("Fade In"));
  }

  RampFactory<float>::Params fadeIn;
};

template<typename T, typename P = EntityPreRendererParams>
class EntityPreRenderer {
public:
  using Params = P;

  EntityPreRenderer(const P& params,
                    Context& context,
                    ObjectManager<T>& entities)
  : _params(params)
  , _context(context)
  , _entities(entities)
  , _fadeIn(params.fadeIn) { }

  virtual void update() = 0;
protected:
  const P& _params;
  Context& _context;
  ObjectManager<T>& _entities;
  RampFactory<float> _fadeIn;
};

#endif /* EntityRenderer_h */
