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

class AttractionBehavior : public Behavior {
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
  AttractionBehavior& setLimit(int limit) {
    _limit = limit;
    return *this;
  }
private:
  float _minDist;
  float _maxDist;
  float _minPull;
  float _maxPull;
  int _limit;
};

#endif /* defined(__behavior__AttractionBehavior__) */
