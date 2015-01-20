//
//  Walker.cpp
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#include "Walker.h"
#include "Common.h"
#include "State.h"

Walker::Walker()
: Entity() {
}

void Walker::update(State &state) {
  noisePos.z += state.timeDelta * 1;
  auto angle = ofSignedNoise(noisePos.x, noisePos.y, noisePos.z) * 180;
  velocity = ofVec3f(0.006);
  velocity.rotate(ofSignedNoise(noisePos.x) * 180,
                  ofSignedNoise(noisePos.y) * 180,
                  ofSignedNoise(noisePos.z) * 180);
  //..
  Entity::update(state);
}
