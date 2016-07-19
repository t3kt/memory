//
//  EntityRenderer.cpp
//  memory
//
//  Created by tekt on 7/2/16.
//
//

#include "AppAssets.h"
#include "EntityRenderer.h"
#include <ofMain.h>

using namespace ofxChoreograph;

void AbstractEntityRenderer::update(const State &state) {
  _fadeIn.update(state);
}
