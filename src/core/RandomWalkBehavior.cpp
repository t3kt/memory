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

RandomWalkBehavior::Params::Params(std::string label) {
  paramGroup.setName(label);
  paramGroup.add(speed.set("Speed", 0.006, 0, 0.1));
}

RandomWalkBehavior::RandomWalkBehavior(const Params& params)
: _params(params) {
  _noisePos = createRandomVec3f(ofVec3f(-1000),
                                ofVec3f(1000));
}

void RandomWalkBehavior::update(Entity& entity,
                                State &state) {
  _noisePos.z += state.timeDelta * 1;
  auto offset = entity.id * 10000;
  auto vel = ofVec3f(_params.speed.get());
  vel.rotate(ofSignedNoise(_noisePos.x + offset) * 180,
             ofSignedNoise(_noisePos.y + offset) * 180,
             ofSignedNoise(_noisePos.z + offset) * 180);
  entity.velocity += vel;
}
