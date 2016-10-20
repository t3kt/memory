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
#include "../control/Params.h"
#include "../control/RampFactory.h"
#include "../control/ValueRange.h"
#include "../core/ObjectManager.h"
#include "../rendering/Colors.h"
#include "../rendering/Renderer.h"

class Context;

class EntityPreRendererParams : public Params {
public:
  EntityPreRendererParams() {
    add(fadeIn
        .setKey("fadeIn")
        .setName("Fade In"));
    add(highlightAmount
        .setKey("highlightAmount")
        .setName("Highlighting")
        .setValueAndDefault(0.4)
        .setRange(0, 1));
  }

  RampFactory<float>::Params fadeIn;
  TParam<float> highlightAmount;
};

template<typename T, typename P = EntityPreRendererParams>
class EntityPreRenderer : public PreRenderer {
public:
  using Params = P;

  EntityPreRenderer(const P& params,
                    const ColorTheme& colors,
                    Context& context,
                    ObjectManager<T>& entities)
  : _params(params)
  , _colors(colors)
  , _context(context)
  , _entities(entities)
  , _fadeIn(params.fadeIn) { }

protected:
  const P& _params;
  const ColorTheme& _colors;
  Context& _context;
  ObjectManager<T>& _entities;
  RampFactory<float> _fadeIn;
};

#endif /* EntityRenderer_h */
