//
//  AttractionBehavior.cpp
//  behavior
//
//  Created by tekt on 1/24/15.
//
//

#include "AttractionBehavior.h"
#include "State.h"
#include <ofVec3f.h>
#include <ofMath.h>

AttractionBehaviorParams::AttractionBehaviorParams() {
  paramGroup.add(minDist.set("Min Dist", 0, 0, 2));
  paramGroup.add(maxDist.set("Max Dist", 0.2, 0, 2));
  paramGroup.add(minPull.set("Min Pull", 0, -.5, .5));
  paramGroup.add(maxPull.set("Max Pull", 0.1, -.5, .5));
}


SingleAttractionBehavior::Params::Params(std::string label) {
  paramGroup.setName(label);
  paramGroup.add(position.set("Position",
                              ofVec3f(0),
                              ofVec3f(-2),
                              ofVec3f(2)));
}

EntityAttractionBehavior::Params::Params(std::string label) {
  paramGroup.setName(label);
  paramGroup.add(limit.set("Limit", 10, 0, 50));
}

static bool applyAttraction(const ofVec3f& attractorPos,
                            Entity* entity,
                            float minDist,
                            float maxDist,
                            float minPull,
                            float maxPull) {
  auto diff = attractorPos - entity->position;
  auto dist = diff.lengthSquared();
  if (dist < minDist || dist > maxDist)
    return false;
  auto pull = ofMap(dist,
                    minDist, maxDist,
                    minPull, maxPull);
  diff.normalize();
  diff *= pull;
  entity->velocity += diff;
  return true;
}

void SingleAttractionBehavior::update(Entity &entity,
                                      State &state) {
  if (!_params.enabled.get())
    return;
  applyAttraction(_params.position.get(), &entity,
                  _params.minDist.get(),
                  _params.maxDist.get(),
                  _params.minPull.get(),
                  _params.maxPull.get());
}

void EntityAttractionBehavior::update(Entity &entity,
                                      State &state) {
  if (!_params.enabled.get())
    return;
  int count = 0;
  auto minDist = _params.minDist.get();
  auto maxDist = _params.maxDist.get();
  auto minPull = _params.minPull.get();
  auto maxPull = _params.maxPull.get();
  auto limit = _params.limit.get();
  for (const auto& other : state.entities) {
    if (other->id == entity.id)
      continue;
    if (applyAttraction(other->position,
                        &entity,
                        minDist, maxDist,
                        minPull, maxPull)) {
      count++;
      if (count >= limit)
        break;
    }
  }
}
