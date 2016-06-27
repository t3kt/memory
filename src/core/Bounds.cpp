//
//  Bounds.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/26/16.
//
//

#include "Bounds.h"

Bounds::Bounds(std::string name)
: Params(name) {
}

SimpleCubeBounds::SimpleCubeBounds(std::string name)
: Bounds(name) {
  add(size.set("Size", 2, 0, 6));
}

static bool reflectVelocity(float *vel, float pos, float minPos, float maxPos) {
  float newPos = pos + *vel;
  if (newPos < minPos || newPos >= maxPos) {
    *vel *= -1;
    return true;
  } else {
    return false;
  }
}

bool SimpleCubeBounds::reflect(ofVec3f *velocity, ofVec3f position) const {
  float bound = size.get() / 2;
  bool changed = false;
  changed |= reflectVelocity(&velocity->x, position.x, -bound, bound);
  changed |= reflectVelocity(&velocity->y, position.y, -bound, bound);
  changed |= reflectVelocity(&velocity->z, position.z, -bound, bound);
  return changed;
}

ofVec3f SimpleCubeBounds::randomPoint() const {
  float bound = size.get() / 2;
  return ofVec3f(ofRandom(-bound, bound),
                 ofRandom(-bound, bound),
                 ofRandom(-bound, bound));
}
