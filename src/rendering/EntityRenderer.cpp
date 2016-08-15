//
//  EntityRenderer.cpp
//  memory
//
//  Created by tekt on 7/2/16.
//
//

#include "../core/Context.h"
#include "../rendering/EntityRenderer.h"

void AbstractEntityRenderer::update() {
  _fadeIn.update(_context.state);
}
