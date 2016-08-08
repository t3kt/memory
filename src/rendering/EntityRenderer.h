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
#include "AnimationObject.h"
#include "Colors.h"
#include "ObjectManager.h"
#include "Params.h"

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

class AbstractEntityRenderer {
public:
  class Params : public ParamsWithEnabled {
  public:
    Params() {
      add(fadeIn
          .setKey("fadeIn")
          .setName("Fade In"));
    }

    RampFactory<float>::Params fadeIn;
  };

  AbstractEntityRenderer(const Params& params,
                         const ofFloatColor& color,
                         Context& context)
  : _baseParams(params)
  , _color(color)
  , _context(context)
  , _fadeIn(params.fadeIn) { }

  virtual void update();
  virtual void draw() = 0;
protected:
  const Params& _baseParams;
  const ofFloatColor& _color;
  Context& _context;
  RampFactory<float> _fadeIn;
};

template<typename T>
class EntityRenderer
: public AbstractEntityRenderer {
public:
  EntityRenderer(const Params& params,
                 const ofFloatColor& color,
                 Context& context,
                 ObjectManager<T>& entities)
  : AbstractEntityRenderer(params,
                           color,
                           context)
  , _entities(entities) { }

  void draw() override {
    if (!_baseParams.enabled.get()) {
      return;
    }
    ofPushStyle();
    ofFill();
    for (const auto& entity : _entities) {
      if (!entity->visible()) {
        continue;
      }
      drawEntity(*entity);
    }
    ofPopStyle();
  }
protected:
  virtual void drawEntity(const T& entity) = 0;

  ObjectManager<T>& _entities;
};

#endif /* EntityRenderer_h */
