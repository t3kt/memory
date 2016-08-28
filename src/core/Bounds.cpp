//
//  Bounds.cpp
//

#include "../core/Bounds.h"

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

bool Bounds::reflect(ofVec3f *velocity, ofVec3f* position) const {
  float bound = size() / 2;
  bool changed = false;
  changed |= reflectVal(&velocity->x, &position->x, -bound, bound);
  changed |= reflectVal(&velocity->y, &position->y, -bound, bound);
  changed |= reflectVal(&velocity->z, &position->z, -bound, bound);
  return changed;
}

ofVec3f Bounds::randomPoint() const {
  float bound = size() / 2;
  return ofVec3f(ofRandom(-bound, bound),
                 ofRandom(-bound, bound),
                 ofRandom(-bound, bound));
}
