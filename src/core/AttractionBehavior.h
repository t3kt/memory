//
//  AttractionBehavior.h
//  behavior
//
//  Created by tekt on 1/24/15.
//
//

#ifndef __behavior__AttractionBehavior__
#define __behavior__AttractionBehavior__

#include "Behavior.h"

class AttractionBehaviorBase : public Behavior {
public:
protected:
  bool applyAttraction(const ofVec3f& attractorPos,
                       Entity* entity) const;
  
  float _minDist;
  float _maxDist;
  float _minPull;
  float _maxPull;
};

class AttractionBehavior : public AttractionBehaviorBase {
public:
  void update(Entity& entity, State& state) override;
  AttractionBehavior& setRange(float min, float max) {
    _minDist = min;
    _maxDist = max;
    return *this;
  }
  AttractionBehavior& setPull(float min, float max) {
    _minPull = min;
    _maxPull = max;
    return *this;
  }
  AttractionBehavior& setPosition(ofVec3f position) {
    _position = position;
    return *this;
  }
private:
  ofVec3f _position;
};

class EntityAttractionBehavior : public AttractionBehaviorBase {
public:
  void update(Entity& entity, State& state) override;
  EntityAttractionBehavior& setRange(float min, float max) {
    _minDist = min;
    _maxDist = max;
    return *this;
  }
  EntityAttractionBehavior& setPull(float min, float max) {
    _minPull = min;
    _maxPull = max;
    return *this;
  }
  EntityAttractionBehavior& setLimit(int limit) {
    _limit = limit;
    return *this;
  }
private:
  int _limit;
};

#endif /* defined(__behavior__AttractionBehavior__) */
