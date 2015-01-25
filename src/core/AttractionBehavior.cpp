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
                            const AttractionBehaviorParams& params,
                            Entity* entity) {
  auto diff = attractorPos - entity->position;
  auto dist = diff.lengthSquared();
  if (dist < params.minDist.get() ||
      dist > params.maxDist.get())
    return false;
  auto pull = ofMap(dist,
                    params.minDist.get(),
                    params.maxDist.get(),
                    params.minPull.get(),
                    params.maxPull.get());
  diff.normalize();
  diff *= pull;
  entity->velocity += diff;
  return true;
}

void SingleAttractionBehavior::update(Entity &entity,
                                      State &state) {
  if (!_params.enabled.get())
    return;
  applyAttraction(_params.position.get(), _params, &entity);
}

void EntityAttractionBehavior::update(Entity &entity,
                                      State &state) {
  if (!_params.enabled.get())
    return;
  int count = 0;
  for (const auto& other : state.entities) {
    if (other->id == entity.id)
      continue;
    if (applyAttraction(other->position,
                        _params,
                        &entity)) {
      count++;
      if (count >= _params.limit)
        break;
    }
  }
}
