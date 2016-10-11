//
//  ForceFieldBehavior.cpp
//  memory
//
//  Created by tekt on 7/7/16.
//
//

#include <ofMain.h>
#include "../physics/ForceFieldBehavior.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"

void AbstractForceFieldBehavior::applyToEntity(ParticleObject *entity) {
  if (!entity->alive()) {
    return;
  }
  ofVec3f force = getForceForEntity(entity);
  entity->addForce(force);
}

void AbstractForceFieldBehavior::beginDebugDraw() {
  ofPushStyle();
  ofSetColor(ofFloatColor::red);
}

void AbstractForceFieldBehavior::debugDrawEntity(ParticleObject *entity) {
  if (!entity->alive()) {
    return;
  }
  if (!_context.highlightedEntities.empty() &&
      !_context.highlightedEntities.containsId(entity->id())) {
    return;
  }
  drawForceArrow(entity->position(),
                 getForceForEntity(entity));
}

void AbstractForceFieldBehavior::endDebugDraw() {
  ofPopStyle();
}

const ofVec4f SPATIAL_NOISE_Y_OFFSET = ofVec4f(100);
const ofVec4f SPATIAL_NOISE_Z_OFFSET = ofVec4f(200);

ofVec3f AbstractNoiseForceFieldBehavior::getForceForEntity(ParticleObject *entity) {
  ofVec4f noisePos = ofVec4f(entity->position() / _params.scale());
  noisePos.w = _context.physicsState.localTime * _params.rate();

  return ofVec3f(ofSignedNoise(noisePos),
                 ofSignedNoise(noisePos + SPATIAL_NOISE_Y_OFFSET),
                 ofSignedNoise(noisePos + SPATIAL_NOISE_Z_OFFSET)) * _params.magnitude();
}
