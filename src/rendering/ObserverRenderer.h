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
#include "AnimationObject.h"
#include "Context.h"
#include "EntityRenderer.h"

class ObserverRenderer
: public EntityRenderer<ObserverEntity> {
public:

  class Params : public AbstractEntityRenderer::Params {
  public:
    Params() {
      add(size
          .setKey("size")
          .setName("Draw Size")
          .setValueAndDefault(10)
          .setRange(0, 100));
    }
    TParam<float> size;
  };

  ObserverRenderer(const Params& params,
                   Context& context);

  void update() override;
protected:
  void drawEntity(const ObserverEntity& entity) override;
private:
  const Params& _params;
  ofVboMesh _mesh;
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
                            Context& context);

  void setup();
  void update();
  void draw();
private:
  Context& _context;
  const Params& _params;
  const ofFloatColor& _color;
  const ofShader& _instanceShader;
  ofVboMesh _mesh;
  std::vector<ofMatrix4x4> _matrices;
  ofBufferObject _matricesBuffer;
  ofTexture _matricesTexture;
  RampFactory<float> _fadeIn;
  std::size_t _count;
};

#endif /* ObserverRenderer_h */
