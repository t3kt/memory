//
//  EntityForceBehavior.cpp
//  memory
//
//  Created by tekt on 8/3/16.
//
//

#include <ofMain.h>
#include "EntityForceBehavior.h"

ofVec3f
ObserverOccurrenceForceBehavior::calcAttraction(const ofVec3f &entityPosition,
                                                const ofVec3f &otherPosition) const {
  ofVec3f posDiff = otherPosition - entityPosition;
  float dist = posDiff.length();
  float forceMag = _params.force.evaluate(dist);
  float magnitude = _params.magnitude.get();
  return posDiff.getNormalized() * forceMag * magnitude;
}

void ObserverOccurrenceForceBehavior::performAction(Context& context,
                                                    Action action) {
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
      action(*entity, *other, force);
    }
  }
}

void ObserverOccurrenceForceBehavior::applyToWorld(Context &context) {
  if (!_params.enabled.get()) {
    return;
  }
  performAction(context, [&](ObserverEntity& entity,
                             OccurrenceEntity& other,
                             const ofVec3f& force) {
    entity.addForce(force);
    other.addForce(-force);
  });
}

void ObserverOccurrenceForceBehavior::beginDebugDraw() {
  ofPushStyle();
  ofSetColor(ofFloatColor::limeGreen);
  //...
}

void ObserverOccurrenceForceBehavior::debugDrawBehavior(Context &context) {
  performAction(context, [&](ObserverEntity& entity,
                             OccurrenceEntity& other,
                             const ofVec3f& force) {
    drawForceArrow(entity.position(), force);
    drawForceArrow(other.position(), -force);
  });
}

void ObserverOccurrenceForceBehavior::endDebugDraw() {
  ofPushStyle();
}
