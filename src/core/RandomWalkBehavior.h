//
//  RandomWalkBehavior.h
//  behavior
//
//  Created by tekt on 1/21/15.
//
//

#ifndef __behavior__RandomWalkBehavior__
#define __behavior__RandomWalkBehavior__

#include <ofVec3f.h>

#include "Behavior.h"

class RandomWalkBehavior : public Behavior {
public:
  RandomWalkBehavior();
  void update(Entity& entity, State& state) override;
private:
  ofVec3f _noisePos;
};

#endif /* defined(__behavior__RandomWalkBehavior__) */
