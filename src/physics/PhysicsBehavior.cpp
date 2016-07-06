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


template<typename E, typename O>
EntityMap<O>& getOthers(E *entity);

template<>
EntityMap<OccurrenceEntity>& getOthers<ObserverEntity, OccurrenceEntity>(ObserverEntity *entity) {
  return entity->getConnectedOccurrences();
}

template<>
EntityMap<ObserverEntity>& getOthers<OccurrenceEntity, ObserverEntity>(OccurrenceEntity *entity) {
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
  _bounds.reflect(entity->velocityPtr(),
                  entity->positionPtr());
}

template<typename E, typename O>
void AttractionBehavior<E, O>::applyToWorld(PhysicsWorld* world) {
  if (!_params.enabled()) {
    return;
  }
  float lowBound = _params.distanceBounds.lowValue();
  float highBound = _params.distanceBounds.highValue();
  float lowMagnitude = _params.forceRange.lowValue();
  float highMagnitude = _params.forceRange.highValue();
  for (auto entity : world->getEntities<E>()) {
    for (auto other : getOthers(entity.get())) {
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

template<typename E>
void SpatialNoiseForceBehavior<E>::applyToWorld(PhysicsWorld *world) {
  if (!_params.enabled()) {
    return;
  }
  for (auto entity : world->getEntities<E>()) {
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
