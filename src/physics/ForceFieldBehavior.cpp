//
//  ForceFieldBehavior.cpp
//  memory
//
//  Created by tekt on 7/7/16.
//
//

#include "ForceFieldBehavior.h"
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include <ofMain.h>

void AbstractForceFieldBehavior::applyToEntity(PhysicsWorld* world, ParticleObject *entity) {
  if (!entity->alive()) {
    return;
  }
  ofVec3f force = getForceForEntity(world, entity);
  entity->addForce(force);
}

void AbstractForceFieldBehavior::beginDebugDraw() {
  ofPushStyle();
  ofSetColor(ofFloatColor::red);
}

void AbstractForceFieldBehavior::debugDrawEntity(PhysicsWorld *world, ParticleObject *entity) {
  if (!entity->alive()) {
    return;
  }
  drawForceArrow(entity->position(),
                 getForceForEntity(world, entity));
}

void AbstractForceFieldBehavior::endDebugDraw() {
  ofPopStyle();
}

const ofVec4f SPATIAL_NOISE_Y_OFFSET = ofVec4f(100);
const ofVec4f SPATIAL_NOISE_Z_OFFSET = ofVec4f(200);

ofVec3f AbstractNoiseForceFieldBehavior::getForceForEntity(PhysicsWorld *world, ParticleObject *entity) {
  ofVec4f noisePos = ofVec4f(entity->position() / _params.scale());
  noisePos.w = world->context().time() * _params.rate();

  return ofVec3f(ofSignedNoise(noisePos),
                 ofSignedNoise(noisePos + SPATIAL_NOISE_Y_OFFSET),
                 ofSignedNoise(noisePos + SPATIAL_NOISE_Z_OFFSET)) * _params.magnitude();
}
