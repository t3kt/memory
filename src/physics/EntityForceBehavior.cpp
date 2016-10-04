//
//  EntityForceBehavior.cpp
//  memory
//
//  Created by tekt on 8/3/16.
//
//

#include <ofGraphics.h>
#include "../physics/EntityForceBehavior.h"

ofVec3f
AbstractEntityForceBehavior::calcAttraction(const ofVec3f &entityPosition,
                                            const ofVec3f &otherPosition) const {
  ofVec3f posDiff = otherPosition - entityPosition;
  float dist = posDiff.length();
  float forceMag = _params.force.evaluate(dist);
  float magnitude = _params.magnitude.get();
  return posDiff.getNormalized() * forceMag * magnitude;
}

void AbstractEntityForceBehavior::beginDebugDraw() {
  ofPushStyle();
  ofSetColor(ofFloatColor::limeGreen);
}

void AbstractEntityForceBehavior::endDebugDraw() {
  ofPopStyle();
}

template<>
void EntityForceBehavior<ObserverEntity, OccurrenceEntity>::performAction(Action action) {
  for (auto& entity : _context.observers) {
    if (!entity->alive()) {
      continue;
    }
    entity->getOccurrenceConnections().performAction([&](std::shared_ptr<OccurrenceEntity> other) {
      if (!other->alive()) {
        return;
      }
      ofVec3f force = calcAttraction(entity->position(),
                                     other->position());
      action(*entity, *other, force);
    });
  }
}

template<>
void EntityForceBehavior<OccurrenceEntity, OccurrenceEntity>::performAction(Action action) {
  for (auto& entity : _context.occurrences) {
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
