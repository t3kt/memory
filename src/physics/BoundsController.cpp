//
//  BoundsController.cpp
//  memory
//
//  Created by tekt on 10/6/16.
//
//

#include <ofGraphics.h>
#include <ofMath.h>
#include "../app/AppParameters.h"
#include "../app/AppSystem.h"
#include "../physics/BoundsController.h"

BoundsController::BoundsController(const Params& params,
                                   DebugParams& debugParams)
: _params(params)
, _debugParams(debugParams)
, _boundsColor(AppSystem::get().params()->colors.bounds.get()) { }

static bool reflectVal(float *vel, float *pos, float minPos, float maxPos) {
  float newPos = *pos + *vel;
  if (newPos < minPos || newPos >= maxPos) {
    *vel *= -1;
    *pos = ofClamp(*pos, minPos, maxPos);
    return true;
  } else {
    return false;
  }
}

bool BoundsController::reflect(ofVec3f *velocity, ofVec3f* position) const {
  float bound = _params.size.get() / 2;
  bool changed = false;
  changed |= reflectVal(&velocity->x, &position->x, -bound, bound);
  changed |= reflectVal(&velocity->y, &position->y, -bound, bound);
  changed |= reflectVal(&velocity->z, &position->z, -bound, bound);
  return changed;
}

ofVec3f BoundsController::randomPoint() const {
  float bound = _params.size.get() / 2;
  return ofVec3f(ofRandom(-bound, bound),
                 ofRandom(-bound, bound),
                 ofRandom(-bound, bound));
}

ofVec3f BoundsController::clampPoint(const ofVec3f &position) const {
  float bound = _params.size.get() / 2;
  return ofVec3f(ofClamp(position.x, -bound, bound),
                 ofClamp(position.y, -bound, bound),
                 ofClamp(position.z, -bound, bound));
}

void BoundsController::draw() {
  if (!_debugParams.showBounds.get()) {
    return;
  }
  auto renderer = ofGetCurrentRenderer();
  renderer->pushStyle();
  renderer->setFillMode(OF_OUTLINE);
  renderer->setColor(_boundsColor);
  renderer->drawBox(_params.size.get());
  renderer->popStyle();
  renderer->drawGrid(_params.size.get() / 2 / 4, // step size
                     4, // number of steps
                     true, // labels
                     true, true, true // x / y /z
                     );
}

bool BoundsController::performAction(AppAction action) {
  switch (action) {
    case AppAction::TOGGLE_SHOW_BOUNDS:
      _debugParams.showBounds.toggle();
      break;
    default:
      return false;
  }
  return true;
}
