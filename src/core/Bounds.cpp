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

bool SimpleCubeBounds::reflect(ofVec3f *velocity, ofVec3f* position) const {
  float bound = size.get() / 2;
  bool changed = false;
  changed |= reflectVal(&velocity->x, &position->x, -bound, bound);
  changed |= reflectVal(&velocity->y, &position->y, -bound, bound);
  changed |= reflectVal(&velocity->z, &position->z, -bound, bound);
  return changed;
}

ofVec3f SimpleCubeBounds::randomPoint() const {
  float bound = size.get() / 2;
  return ofVec3f(ofRandom(-bound, bound),
                 ofRandom(-bound, bound),
                 ofRandom(-bound, bound));
}
