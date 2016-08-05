//
//  EntityForceBehavior.cpp
//  memory
//
//  Created by tekt on 8/3/16.
//
//

#include "EntityForceBehavior.h"

ofVec3f
ObserverOccurrenceForceBehavior::calcAttraction(const ofVec3f &entityPosition,
                                                const ofVec3f &otherPosition) const {
  ofVec3f posDiff = otherPosition - entityPosition;
  float dist = posDiff.length();
  float magnitude = _params.force.evaluate(dist);
  return posDiff.getNormalized() * magnitude;
}

void ObserverOccurrenceForceBehavior::applyToWorld(Context &context) {
  if (!_params.enabled.get()) {
    return;
  }
  for (auto& entity : context.observers) {
    if (!entity->alive()) {
      continue;
    }
    for (auto& otherEntry : entity->getConnectedOccurrences()) {
      auto& other = otherEntry.second;
      if (!other->alive()) {
        continue;
      }
      ofVec3f force = calcAttraction(entity->position(),
                                     other->position());
      entity->addForce(force);
      other->addForce(-force);
    }
  }
}
