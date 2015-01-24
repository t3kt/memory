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

bool
AttractionBehaviorBase::applyAttraction(const ofVec3f& attractorPos,
                                        Entity *entity) const {
  auto diff = attractorPos - entity->position;
  auto dist = diff.lengthSquared();
  if (dist < _minDist || dist > _maxDist)
    return false;
  auto pull = ofMap(dist, _minDist, _maxDist,
                    _minPull, _maxPull);
  diff.normalize();
  diff *= pull;
  entity->velocity += diff;
  return true;
}

void AttractionBehavior::update(Entity &entity, State &state) {
  applyAttraction(_position, &entity);
}

void EntityAttractionBehavior::update(Entity &entity,
                                      State &state) {
  int count = 0;
  for (const auto& other : state.entities) {
    if (other->id == entity.id)
      continue;
    if (applyAttraction(other->position,
                        &entity)) {
      count++;
      if (count >= _limit)
        break;
    }
  }
}
