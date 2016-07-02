//
//  Bounds.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/26/16.
//
//

#include "Bounds.h"
#include "ParamsGui.h"

Bounds::Bounds()
: Params() {
}

SimpleCubeBounds::SimpleCubeBounds()
: Bounds() {
  add(size
      .setKey("size")
      .setName("Size")
      .setValueAndDefault(6)
      .setRange(0, 10));
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

class SimpleCubeBoundsGui
: public AbstractParamsGui {
public:
  SimpleCubeBoundsGui(SimpleCubeBounds& params)
  : AbstractParamsGui(params)
  , _params(params) { }
protected:
  void addControls() override {
    addSlider(_params.size);
  }
private:
  SimpleCubeBounds& _params;
};

ParamsGui* SimpleCubeBounds::createGui() {
  return new SimpleCubeBoundsGui(*this);
}
