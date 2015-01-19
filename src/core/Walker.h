//
//  Walker.h
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#ifndef __behavior__Walker__
#define __behavior__Walker__

#include <ofVec3f.h>

#include "Entity.h"

class Walker : public Entity {
public:
  Walker();
  ~Walker() override { }
  void update(State& state) override;
private:
  ofVec3f _noisePos;
};

#endif /* defined(__behavior__Walker__) */
