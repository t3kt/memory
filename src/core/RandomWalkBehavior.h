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
#include <ofParameterGroup.h>

#include "Behavior.h"

class RandomWalkBehavior : public Behavior {
public:
  class Params {
  public:
    Params(std::string label);
    
    Params& setSpeed(float sp) {
      speed.set(sp);
      return *this;
    }
    
    ofParameter<float> speed;
    ofParameterGroup paramGroup;
  };
  
  explicit RandomWalkBehavior(const Params& params);
  void update(Entity& entity, State& state) override;
private:
  const Params& _params;
  ofVec3f _noisePos;
};

#endif /* defined(__behavior__RandomWalkBehavior__) */
