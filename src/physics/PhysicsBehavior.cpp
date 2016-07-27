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
  force *= 20.0f;
  ofDrawArrow(position,
              position + force,
              force.length() * 0.1f);
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
  if (_bounds.reflect(entity->velocityPtr(),
                      entity->positionPtr())) {
    AppSystem::get().log().app().logNotice([&](ofLog& log) {
      log << "Observer rebounded: " << *entity;
    });
  }
}
