//
//  ObserverRenderer.h
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#ifndef ObserverRenderer_h
#define ObserverRenderer_h

#include <ofBufferObject.h>
#include <ofMatrix4x4.h>
#include <ofShader.h>
#include <ofTexture.h>
#include <ofVboMesh.h>
#include <vector>
#include "../core/AnimationObject.h"
#include "../core/Context.h"
#include "../rendering/EntityRenderer.h"

class ObserverRenderer {
public:

  class Params : public ParamsWithEnabled {
  public:
    Params() {
      add(size
          .setKey("size")
          .setName("Draw Size")
          .setValueAndDefault(2)
          .setRange(0, 20));
      add(highlightAmount
          .setKey("highlightAmount")
          .setName("Highlighting")
          .setValueAndDefault(0.6)
          .setRange(0, 1));
    }
    TParam<float> size;
    TParam<float> highlightAmount;
  };

  ObserverRenderer(const Params& params,
                   const ColorTheme& colors,
                   Context& context);

  void draw();

private:
  Context& _context;
  const Params& _params;
  const ColorTheme& _colors;
  ObjectManager<ObserverEntity>& _entities;
};

class InstancedObserverRenderer {
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

  InstancedObserverRenderer(const Params& params,
                            const ColorTheme& colors,
                            Context& context);

  void setup();
  void update();
  void draw();
private:
  Context& _context;
  const Params& _params;
  const ColorTheme& _colors;
  const ofShader& _instanceShader;
  ofVboMesh _mesh;
  std::vector<ofMatrix4x4> _matrices;
  ofBufferObject _matricesBuffer;
  ofTexture _matricesTexture;
  RampFactory<float> _fadeIn;
  std::size_t _count;
};

#endif /* ObserverRenderer_h */
