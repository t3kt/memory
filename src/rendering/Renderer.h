//
//  Renderer.h
//  memory
//
//  Created by tekt on 9/23/16.
//
//

#ifndef Renderer_h
#define Renderer_h

#include "../core/Component.h"

class Renderer
: public ComponentBase { };

class PreRenderer
: public ComponentBase {
public:
  void draw() final { }
};

using PreRendererCollection = ComponentCollection<PreRenderer>;
using RendererCollection = ComponentCollection<Renderer>;

#endif /* Renderer_h */
