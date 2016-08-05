//
//  PhysicsBehavior.cpp
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#include "AppSystem.h"
#include "PhysicsBehavior.h"
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include <ofMain.h>

void AbstractPhysicsBehavior::drawForceArrow(ofVec3f position,
                                             ofVec3f force) {
  ofDrawArrow(position,
              position + force * 20.0f,
              ofClamp(force.length() * 2.0f,
                      0,
                      6.0f));
}

void BoundsBehavior::applyToWorld(Context& context) {
  std::function<void(ParticleObject*)> action = [&](ParticleObject* entity) {
    applyToEntity(context, entity);
  };
  context.observers.performTypedAction(action);
  context.occurrences.performTypedAction(action);
}

void BoundsBehavior::applyToEntity(Context& context,
                                   ParticleObject *entity) {
  if (!entity->alive()) {
    return;
  }
  _bounds.reflect(entity->velocityPtr(),
                  entity->positionPtr());
}
