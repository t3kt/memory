//
//  EntityRenderer.cpp
//  memory
//
//  Created by tekt on 7/2/16.
//
//

#include "EntityRenderer.h"

void AbstractEntityRenderer::update(const State &state) {
  _fadeIn.update(state);
}
