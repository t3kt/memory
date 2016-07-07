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


template<>
EntityMap<OccurrenceEntity>& AbstractAttractionBehavior::getEntityOthers<ObserverEntity, OccurrenceEntity>(ObserverEntity *entity) {
  return entity->getConnectedOccurrences();
}

template<>
EntityMap<ObserverEntity>& AbstractAttractionBehavior::getEntityOthers<OccurrenceEntity, ObserverEntity>(OccurrenceEntity *entity) {
  return entity->connectedObservers();
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

const ofVec4f SPATIAL_NOISE_Y_OFFSET = ofVec4f(100);
const ofVec4f SPATIAL_NOISE_Z_OFFSET = ofVec4f(200);

ofVec3f AbstractSpatialNoiseForceBehavior::getForceForEntity(PhysicsWorld *world, ParticleObject *entity) {
  ofVec4f noisePos = ofVec4f(entity->position() / _params.scale());
  noisePos.w = world->time() * _params.rate();

  return ofVec3f(ofSignedNoise(noisePos),
                 ofSignedNoise(noisePos + SPATIAL_NOISE_Y_OFFSET),
                 ofSignedNoise(noisePos + SPATIAL_NOISE_Z_OFFSET));
}

void AbstractSpatialNoiseForceBehavior::applyToEntity(PhysicsWorld* world, ParticleObject *entity) {
  if (!entity->alive()) {
    return;
  }
  ofVec3f force = getForceForEntity(world, entity);
  entity->addForce(force);
}

void AbstractSpatialNoiseForceBehavior::beginDebugDraw() {
  ofPushStyle();
  ofSetColor(ofFloatColor::red);
}

void AbstractSpatialNoiseForceBehavior::debugDrawEntity(PhysicsWorld *world, ParticleObject *entity) {
  if (!entity->alive()) {
    return;
  }
  ofVec3f force = getForceForEntity(world, entity);
  ofVec3f start = entity->position();
  ofVec3f end = start + force;
  ofDrawArrow(start, end);
}

void AbstractSpatialNoiseForceBehavior::endDebugDraw() {
  ofPopStyle();
}
