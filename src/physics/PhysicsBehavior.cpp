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

void BoundsBehavior::update() {
  PtrAction<ParticleObject> action = [&](std::shared_ptr<ParticleObject> entity) {
    applyToEntity(entity.get());
  };
  _context.observers.performAction(action);
  _context.occurrences.performAction(action);
}

void BoundsBehavior::applyToEntity(ParticleObject *entity) {
  if (!entity->alive()) {
    return;
  }
  _bounds.reflect(entity->velocityPtr(),
                  entity->positionPtr());
}
