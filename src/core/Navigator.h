//
//  Navigator.h
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#ifndef Navigator_h
#define Navigator_h

#include <memory>
#include <ofVec3f.h>
#include "AnimationObject.h"
#include "ObserverEntity.h"
#include "ObjectManager.h"
#include "OccurrenceEntity.h"
#include "State.h"
#include "WorldObject.h"

class NavStep;

class NavContext {
public:
  NavContext(const State& state)
  : _state(state) { }

  const State& state() const { return _state; }
  float time() const { return _state.time; }
  const ofVec3f& position() const { return _position; }
  void setPosition(ofVec3f position) { _position = position; }
  ofVec3f* positionPtr() { return &_position; }
private:
  const State& _state;
  ofVec3f _position;
};

class NavLocation {
public:
  enum class Type {
    ANIMATION,
    OBSERVER,
    OCCURRENCE,
    POINT,
  };

  virtual WorldObject* object() = 0;
  virtual const WorldObject* object() const = 0;
  virtual const ofVec3f& position() const = 0;
  virtual Type type() const = 0;

  virtual NavStep nextStep(NavContext& context) = 0;
};

using NavLocationPtr = std::shared_ptr<NavLocation>;

class NavStep {
public:
  NavStep()
  : _location(nullptr)
  , _time(0) { }

  NavStep(NavLocationPtr location, float time)
  : _location(location)
  , _time(time) { }

  NavLocationPtr location() { return _location; }
  const NavLocationPtr location() const { return _location; }
  float time() const { return _time; }
  const ofVec3f& position() const { return _location->position(); }

  operator bool() const { return !_location; }
private:
  NavLocationPtr _location;
  float _time;
};

class Navigator {
public:
  Navigator(const State& state)
  : _context(state) { }

  void setup();

  void update();

  void jumpTo(AnimationObject& entity);
  void jumpTo(ObserverEntity& entity);
  void jumpTo(OccurrenceEntity& entity);
  void jumpTo(const ofVec3f& point);

  const NavLocation* prevLocation() const {
    return _prevStep.location().get();
  }

  const NavLocation* nextLocation() const {
    return _nextStep.location().get();
  }

  const ofVec3f& position() const { return _context.position(); }
  const ofVec3f& prevPosition() const {
    return _prevStep ? _prevStep.position() : _context.position();
  }

  const ofVec3f& nextPosition() const {
    return _nextStep ? _nextStep.position() : _context.position();
  }
private:
  void jumpTo(NavLocationPtr location);

  NavContext _context;
  NavStep _prevStep;
  NavStep _nextStep;
};

#endif /* Navigator_h */
