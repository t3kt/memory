//
//  EntityRenderer.cpp
//  memory
//
//  Created by tekt on 7/2/16.
//
//

#include "Context.h"
#include "EntityRenderer.h"

void AbstractEntityRenderer::update() {
  _fadeIn.update(_context.state);
}
