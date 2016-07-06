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

template<>
void SpatialNoiseForceBehavior<ObserverEntity>::applyToWorld(PhysicsWorld *world) {
  if (!_params.enabled()) {
    return;
  }
  for (auto entity : world->observers()) {
    applyToEntity(world, entity.get());
  }
}

template<>
void SpatialNoiseForceBehavior<OccurrenceEntity>::applyToWorld(PhysicsWorld *world) {
  if (!_params.enabled()) {
    return;
  }
  for (auto entity : world->occurrences()) {
    applyToEntity(world, entity.get());
  }
}

const ofVec4f SPATIAL_NOISE_Y_OFFSET = ofVec4f(100);
const ofVec4f SPATIAL_NOISE_Z_OFFSET = ofVec4f(200);

void AbstractSpatialNoiseForceBehavior::applyToEntity(PhysicsWorld* world, ParticleObject *entity) {
  ofVec4f noisePos = ofVec4f(entity->position() * _params.scale());
  noisePos.w = world->time() * _params.rate();

  ofVec3f force(ofSignedNoise(noisePos),
                ofSignedNoise(noisePos + SPATIAL_NOISE_Y_OFFSET),
                ofSignedNoise(noisePos + SPATIAL_NOISE_Z_OFFSET));
  entity->addForce(force);
}
