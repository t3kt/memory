//
//  ConnectionTracerRenderer.h
//  memory
//
//  Created by tekt on 10/7/16.
//
//

#ifndef ConnectionTracerRenderer_h
#define ConnectionTracerRenderer_h

#include "../core/Connection.h"
#include "../core/Params.h"
#include "../core/ParticleObject.h"
#include "../rendering/Renderer.h"

class Context;

class ConnectionTracerRendererParams
: public ParamsWithEnabled {
public:
  ConnectionTracerRendererParams() {
    add(drawRadius
        .setKey("drawRadius")
        .setName("Draw Radius")
        .setRange(0, 10)
        .setDefaultValue(1));
    add(rate
        .setKey("rate")
        .setName("Rate")
        .setRange(0, 2)
        .setDefaultValue(0.4));
  }

  TParam<float> drawRadius;
  TParam<float> rate;
};

class ConnectionTracerRenderer
: public Renderer {
public:
  using Params = ConnectionTracerRendererParams;

  ConnectionTracerRenderer(const Params& params,
                           Context& context)
  : _params(params)
  , _context(context) { }

  void update() override;
  void draw() override;

private:
  void drawTracer(ParticleObject& entityA,
                  ParticleObject& entityB,
                  std::shared_ptr<ofBaseRenderer>& renderer);

  const Params& _params;
  Context& _context;
  float _rawRatio;
  float _ratio;
};

#endif /* ConnectionTracerRenderer_h */
