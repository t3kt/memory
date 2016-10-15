//
//  ConnectionTracerRenderer.h
//  memory
//
//  Created by tekt on 10/7/16.
//
//

#ifndef ConnectionTracerRenderer_h
#define ConnectionTracerRenderer_h

#include "../control/Params.h"
#include "../control/ValueSequence.h"
#include "../core/Connection.h"
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
    add(alphaFade
        .setKey("alphaFade")
        .setName("Alpha Fade"));
    alphaFade.setValueRanges(0, 1);
    alphaFade.setLengthRanges(0, 1);
    alphaFade.startValue.setValueAndDefault(0);
    alphaFade.values[0].setValueAndDefault(1);
    alphaFade.values[1].setValueAndDefault(1);
    alphaFade.values[2].setValueAndDefault(0);
    alphaFade.lengths[0].setValueAndDefault(0.3);
    alphaFade.lengths[1].setValueAndDefault(0.7);
    alphaFade.lengths[2].setValueAndDefault(1);
  }

  TParam<float> drawRadius;
  TParam<float> rate;
  ValueSequence<float, 3> alphaFade;
};

class ConnectionTracerRenderer
: public Renderer {
public:
  using Params = ConnectionTracerRendererParams;

  ConnectionTracerRenderer(const Params& params,
                           Context& context)
  : _params(params)
  , _context(context)
  , _rawRatio(0) { }

  void update() override;
  void draw() override;

private:
  void drawTracer(ParticleObject& entityA,
                  ParticleObject& entityB,
                  std::shared_ptr<ofBaseRenderer>& renderer);

  const Params& _params;
  Context& _context;
  float _rawRatio;
};

#endif /* ConnectionTracerRenderer_h */
