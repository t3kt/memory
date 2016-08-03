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

#include "Params.h"
#include "Colors.h"
#include "State.h"
#include "ObjectManager.h"
#include "AnimationObject.h"

class AbstractEntityRenderer {
public:
  class Params : public ParamsWithEnabled {
  public:
    Params() {
      add(size
          .setKey("size")
          .setName("Draw Size")
          .setValueAndDefault(10)
          .setRange(0, 100));
      add(fadeIn
          .setKey("fadeIn")
          .setName("Fade In"));
    }

    RampFactory<float>::Params fadeIn;
    TParam<float> size;
  };

  AbstractEntityRenderer(const Params& params, const ofFloatColor& color)
  : _baseParams(params)
  , _color(color)
  , _fadeIn(params.fadeIn) { }

  virtual void update(const State& state);
  virtual void draw(const State& state) = 0;
protected:
  const Params& _baseParams;
  const ofFloatColor& _color;
  RampFactory<float> _fadeIn;
};

template<typename T>
class EntityRenderer
: public AbstractEntityRenderer {
public:
  EntityRenderer(const Params& params,
                 const ofFloatColor& color)
  : AbstractEntityRenderer(params, color) { }

  void draw(const State& state) override {
    if (!_baseParams.enabled.get()) {
      return;
    }
    ofPushStyle();
    ofFill();
    for (std::shared_ptr<T> entity : *this) {
      if (!entity->visible()) {
        continue;
      }
      drawEntity(*entity, state);
    }
    ofPopStyle();
  }
protected:
  virtual typename ObjectManager<T>::StorageList::iterator begin() = 0;
  virtual typename ObjectManager<T>::StorageList::iterator end() = 0;
  virtual void drawEntity(const T& entity, const State& state) = 0;
};

#endif /* EntityRenderer_h */
