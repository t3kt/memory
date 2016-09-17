//
//  PhysicsBehavior.cpp
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#include <ofMain.h>
#include "../app/AppSystem.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"
#include "../physics/PhysicsBehavior.h"

void AbstractPhysicsBehavior::drawForceArrow(ofVec3f position,
                                             ofVec3f force) {
  ofDrawArrow(position,
              position + force * 20.0f,
              ofClamp(force.length() * 2.0f,
                      0,
                      6.0f));
}

void BoundsBehavior::applyToWorld(Context& context) {
  PtrAction<ParticleObject> action = [&](std::shared_ptr<ParticleObject> entity) {
    applyToEntity(context, entity.get());
  };
  context.observers.performAction(action);
  context.occurrences.performAction(action);
}

void BoundsBehavior::applyToEntity(Context& context,
                                   ParticleObject *entity) {
  if (!entity->alive()) {
    return;
  }
  _bounds.reflect(entity->velocityPtr(),
                  entity->positionPtr());
}
