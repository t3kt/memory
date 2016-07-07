//
//  PhysicsBehavior.cpp
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#include "PhysicsBehavior.h"
#include <ofLog.h>


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
  if (_bounds.reflect(entity->velocityPtr(),
                      entity->positionPtr())) {
    ofLogNotice() << "Observer rebounded: " << *entity;
  }
}

template<>
void AttractionBehavior<ObserverEntity, OccurrenceEntity>::applyToWorld(PhysicsWorld* world) {
  if (!_params.enabled()) {
    return;
  }
  float lowBound = _params.distanceBounds.lowValue();
  float highBound = _params.distanceBounds.highValue();
  float lowMagnitude = _params.forceRange.lowValue();
  float highMagnitude = _params.forceRange.highValue();
  for (auto entity : world->getEntities<ObserverEntity>()) {
    for (auto other : getEntityOthers<ObserverEntity, OccurrenceEntity>(entity.get())) {
      ofVec3f posDiff = other.second->position() - entity->position();
      float dist = posDiff.length();
      if (dist < lowBound || dist > highBound) {
        continue;
      }
      float mag = ofMap(dist, lowBound, highBound, lowMagnitude, highMagnitude, true);
      posDiff.normalize();
      entity->addForce(posDiff * mag);
    }
  }
}

template<>
void AttractionBehavior<OccurrenceEntity, ObserverEntity>::applyToWorld(PhysicsWorld* world) {
  if (!_params.enabled()) {
    return;
  }
  float lowBound = _params.distanceBounds.lowValue();
  float highBound = _params.distanceBounds.highValue();
  float lowMagnitude = _params.forceRange.lowValue();
  float highMagnitude = _params.forceRange.highValue();
  for (auto entity : world->getEntities<OccurrenceEntity>()) {
    for (auto other : getEntityOthers<OccurrenceEntity, ObserverEntity>(entity.get())) {
      ofVec3f posDiff = other.second->position() - entity->position();
      float dist = posDiff.length();
      if (dist < lowBound || dist > highBound) {
        continue;
      }
      float mag = ofMap(dist, lowBound, highBound, lowMagnitude, highMagnitude, true);
      posDiff.normalize();
      entity->addForce(posDiff * mag);
    }
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
