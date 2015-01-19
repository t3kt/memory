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
  _noisePos = createRandomVec3f(100000);
  position = createSignedNoiseVec3f(_noisePos);
}

void Walker::update(State &state) {
  _noisePos.z += state.timeDelta * 1;
  auto angle = ofSignedNoise(_noisePos.x, _noisePos.y, _noisePos.z) * 180;
  velocity = ofVec3f(0.006);
  velocity.rotate(ofSignedNoise(_noisePos.x) * 180,
                  ofSignedNoise(_noisePos.y) * 180,
                  ofSignedNoise(_noisePos.z) * 180);
  //..
  Entity::update(state);
}
