//
//  PhysicsBehavior.cpp
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#include "PhysicsBehavior.h"
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include <ofMain.h>

void AbstractPhysicsBehavior::drawForceArrow(ofVec3f position,
                                             ofVec3f force) {
  force *= 20.0f;
  ofDrawArrow(position,
              position + force,
              force.length() * 0.1f);
}

void BoundsBehavior::applyToWorld(PhysicsWorld *world) {
  world->performActionOnAllEntities([&](ParticleObject* entity) {
    applyToEntity(world, entity);
  });
}

void BoundsBehavior::applyToEntity(PhysicsWorld *world,
                                   ParticleObject *entity) {
  if (!entity->alive()) {
    return;
  }
  if (_bounds.reflect(entity->velocityPtr(),
                      entity->positionPtr())) {
    ofLogNotice() << "Observer rebounded: " << *entity;
  }
}
