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

static void drawForceArrow(ofVec3f position,
                           ofVec3f force) {
  force *= 70.0f;
  ofDrawArrow(position,
              position + force,
              force.length() * 0.05f);
}

template<>
EntityMap<OccurrenceEntity>& AttractionBehavior<ObserverEntity, OccurrenceEntity>::getEntityOthers(ObserverEntity *entity) {
  return entity->getConnectedOccurrences();
}

template<>
EntityMap<ObserverEntity>& AttractionBehavior<OccurrenceEntity, ObserverEntity>::getEntityOthers(OccurrenceEntity *entity) {
  return entity->connectedObservers();
}

void AbstractAttractionBehavior::beginDebugDraw() {
  ofPushStyle();
  ofSetColor(ofFloatColor::green);
}

void AbstractAttractionBehavior::debugDrawEntity(ParticleObject *entity,
                                                 const ofVec3f &force) {
  drawForceArrow(entity->position(),
                 force);
  debugDrawEntityRange(entity);
}

void AbstractAttractionBehavior::debugDrawEntityRange(ParticleObject *entity) {
  ofPushStyle();
  ofEnableAlphaBlending();
  ofSetColor(ofFloatColor(0.9, 0.1, 0.1, 0.6));
  ofDrawSphere(entity->position(), _params.distanceBounds.lowValue());
  ofSetColor(ofFloatColor(0.1, 0.1, 0.9, 0.4));
  ofDrawSphere(entity->position(), _params.distanceBounds.highValue());
  ofPopStyle();
}

void AbstractAttractionBehavior::endDebugDraw() {
  ofPopStyle();
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
                 ofSignedNoise(noisePos + SPATIAL_NOISE_Z_OFFSET)) * _params.magnitude();
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
  drawForceArrow(entity->position(),
                 getForceForEntity(world, entity));
}

void AbstractSpatialNoiseForceBehavior::endDebugDraw() {
  ofPopStyle();
}
