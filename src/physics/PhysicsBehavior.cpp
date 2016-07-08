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
  force *= 70.0f;
  ofDrawArrow(position,
              position + force,
              force.length() * 0.05f);
}

void BoundsBehavior::applyToWorld(PhysicsWorld *world) {
  for (auto entity : world->observers()) {
    applyToEntity(world, entity.get());
  }
  for (auto entity : world->occurrences()) {
    applyToEntity(world, entity.get());
  }
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
