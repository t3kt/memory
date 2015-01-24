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

void AttractionBehavior::update(Entity &entity, State &state) {
  ofVec3f vel;
  int count = 0;
  for (const auto& other : state.entities) {
    if (other->id == entity.id)
      continue;
    auto diff = other->position - entity.position;
    auto dist = diff.length();
    if (dist >= _minDist && dist <= _maxDist) {
      auto pull = ofMap(dist, _minDist, _maxDist,
                        _minPull, _maxPull);
      diff.normalize();
      diff *= pull;
      vel += diff;
      count++;
      if (count >= _limit)
        break;
    }
  }
  entity.velocity += vel;
}
