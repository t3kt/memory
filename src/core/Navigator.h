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

class NavLocation;
using NavLocationPtr = std::shared_ptr<NavLocation>;

class NavStep {
public:
  static NavStep none() { return NavStep(nullptr, 0); }

  NavStep(NavLocationPtr location, float duration)
  : _location(location)
  , _duration(duration) { }

  NavLocationPtr location() { return _location; }
  float duration() const { return _duration; }

  bool isNone() const { return !_location; }
private:
  NavLocationPtr _location;
  float _duration;
};

class NavContext {
public:
  NavContext(const State& state)
  : _state(state) { }

  const State& state() const { return _state; }
private:
  const State& _state;
};

enum class NavLocationType {
  ANIMATION,
  OBSERVER,
  OCCURRENCE,
  POINT,
};

class NavLocation {
public:
  virtual WorldObject* object() = 0;
  virtual const WorldObject* object() const = 0;
  virtual const ofVec3f& position() const = 0;
  virtual NavLocationType type() const = 0;

  virtual NavStep nextStep(NavContext& context) = 0;
};

class Navigator {
public:
  Navigator(const State& state)
  : _context(state) { }

  void setup();

  void update();

  void jumpTo(const AnimationObject& entity);
  void jumpTo(const ObserverEntity& entity);
  void jumpTo(const OccurrenceEntity& entity);
  void jumpTo(const ofVec3f& point);

  const NavLocation& currentLocation() const {
    return *_currentLocation;
  }
private:
  void jumpTo(NavLocationPtr location);

  NavContext _context;
  NavLocationPtr _currentLocation;
  NavLocationPtr _nextLocation;
};

#endif /* Navigator_h */
