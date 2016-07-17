//
//  Navigator.cpp
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#include <ofMath.h>
#include "AppSystem.h"
#include "Common.h"
#include "Navigator.h"

static ofVec3f randomPointInBounds() {
  return AppSystem::get().params()->core.bounds.randomPoint();
}

class PointLocation
: public NavLocation {
public:
  PointLocation(ofVec3f point)
  : _point(point) {}

  WorldObject* object() override { return nullptr; }
  const WorldObject* object() const override { return nullptr; }
  const ofVec3f& position() const override { return _point; }
  Type type() const override { return Type::POINT; }

  NavStep nextStep(NavContext& context) override {
    const float STEP_RATE = 8;
    //.....
    auto nextPoint = randomPointInBounds();
    auto dist = (nextPoint - _point).length();
    return NavStep(std::make_shared<PointLocation>(nextPoint),
                   context.time() + (dist * STEP_RATE));
  }

private:
  ofVec3f _point;
};

void Navigator::setup() {
  jumpTo(ofVec3f::zero());
}

void Navigator::jumpToRandomPoint() {
  jumpTo(randomPointInBounds());
}

void Navigator::jumpTo(const ofVec3f &point) {
  jumpTo(std::make_shared<PointLocation>(point));
}

void Navigator::jumpTo(NavLocationPtr location) {
  _prevStep = NavStep(location, _context.state().time);
  _context.setPosition(location->position());
  _nextStep = location->nextStep(_context);
}

void Navigator::update() {
  if (!_prevStep || !_nextStep) {
    return;
  }
  float now = _context.time();
  float nextTime = _nextStep.time();

  if (now >= nextTime) {
    jumpTo(_nextStep.location());
  } else {
    float prevTime = _prevStep.time();
    float percent = ofMap(now,
                          prevTime, nextTime,
                          0, 1);
    _context.setPosition(getInterpolated(prevPosition(),
                                         nextPosition(),
                                         percent));
  }
}
