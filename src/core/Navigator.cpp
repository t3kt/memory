//
//  Navigator.cpp
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#include <iterator>
#include <ofMath.h>
#include "AppSystem.h"
#include "Common.h"
#include "Navigator.h"

static ofVec3f randomPointInBounds() {
  return AppSystem::get().params()->core.bounds.randomPoint();
}

template<typename E>
std::shared_ptr<E> getRandomEntity(EntityMap<E>& entities) {
  if (entities.empty()) {
    return std::shared_ptr<E>();
  }
  auto index = static_cast<int>(ofRandom(0, entities.size() - 1));
  return entities.getAtIndex(index);
}

template<typename E>
std::shared_ptr<E> getRandomEntity(ObjectManager<E>& entities) {
  if (entities.empty()) {
    return std::shared_ptr<E>();
  }
  auto index = static_cast<int>(ofRandom(0, entities.size() - 1));
  return entities.getAtIndex(index);
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
    const float STEP_RATE = 30;
    //.....
    auto nextPoint = randomPointInBounds();
    auto dist = (nextPoint - _point).length();
    return NavStep(std::make_shared<PointLocation>(nextPoint),
                   context.time() + (dist * STEP_RATE));
  }

private:
  ofVec3f _point;
};

template <typename E, NavLocation::Type T>
class EntityLocation
: public NavLocation {
public:
  EntityLocation(std::shared_ptr<E> entity)
  : _entity(entity) { }

  WorldObject* object() override { return _entity.get(); }
  const WorldObject* object() const override { return _entity.get(); }
  const ofVec3f& position() const override { return _entity->position(); }
  Type type() const override { return T; }

protected:
  std::shared_ptr<E> _entity;
};

class ObserverLocation
: public EntityLocation<ObserverEntity, NavLocation::Type::OBSERVER> {
public:
  ObserverLocation(std::shared_ptr<ObserverEntity> entity)
  : EntityLocation(entity) { }

  NavStep nextStep(NavContext& context) override {
    const float STEP_RATE = 8;

    auto other = getRandomEntity(_entity->getConnectedObservers());
    if (!other) {
      return NavStep();
    }
    auto dist = (other->position() - _entity->position()).length();
    return NavStep(std::make_shared<ObserverLocation>(other),
                   context.time() + (dist * STEP_RATE));
  }
};
void Navigator::jumpToRandomPoint() {
  jumpTo(randomPointInBounds());
}

void Navigator::jumpTo(const ofVec3f &point) {
  jumpTo(std::make_shared<PointLocation>(point));
}

void Navigator::jumpToRandomObserver() {
  if (_context.context().observers.empty()) {
    return;
  }
  auto entity = getRandomEntity(_context.context().observers);
  if (entity) {
    jumpTo(std::make_shared<ObserverLocation>(entity));
  }
}

void Navigator::jumpTo(NavLocationPtr location) {
  _prevStep = NavStep(location, _context.context().time());
  _context.setPosition(location->position());
  _nextStep = location->nextStep(_context);
}

void Navigator::update() {
  if (!_prevStep) {
    return;
  }
  _nextStep = _prevStep.location()->nextStep(_context);
  if (!_nextStep) {
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
