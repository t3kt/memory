//
//  RandomWalkBehavior.cpp
//  behavior
//
//  Created by tekt on 1/21/15.
//
//

#include "RandomWalkBehavior.h"
#include "Common.h"
#include "State.h"

RandomWalkBehavior::RandomWalkBehavior() {
  _noisePos = createRandomVec3f(ofVec3f(-1000),
                                ofVec3f(1000));
}

void RandomWalkBehavior::update(Entity& entity,
                                State &state) {
  _noisePos.z += state.timeDelta * 1;
  auto angle = ofSignedNoise(_noisePos.x,
                             _noisePos.y,
                             _noisePos.z) * 180;
  auto vel = ofVec3f(0.006);
  vel.rotate(ofSignedNoise(_noisePos.x) * 180,
             ofSignedNoise(_noisePos.y) * 180,
             ofSignedNoise(_noisePos.z) * 180);
  entity.velocity += vel;
}
