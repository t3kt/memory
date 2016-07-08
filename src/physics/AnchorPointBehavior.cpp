//
//  AnchorPointBehavior.cpp
//  memory
//
//  Created by tekt on 7/7/16.
//
//

#include "AnchorPointBehavior.h"
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include <ofMain.h>

template<>
void AnchorPointBehavior<ObserverEntity>::drawOriginShape(const ofVec3f &position) {
  ofDrawSphere(position, 0.04);
}

template<>
void AnchorPointBehavior<OccurrenceEntity>::drawOriginShape(const ofVec3f &position) {
  ofDrawBox(position, 0.04);
}

void AbstractAnchorPointBehavior::debugDrawEntity(ParticleObject *entity, const ofVec3f &force) {
  AbstractAttractionBehavior::debugDrawEntity(entity, force);

  ofPushStyle();

  ofSetColor(ofFloatColor(0.2, 0.2, 0.2, 0.6));
  drawOriginShape(entity->startPosition());
  ofSetLineWidth(2);
  ofDrawLine(entity->startPosition(), entity->position());

  ofPopStyle();
}

ofVec3f AbstractAnchorPointBehavior::calcAttractionForce(ParticleObject *entity, const ofVec3f &otherPosition) {
  ofVec3f posDiff = otherPosition - entity->position();
  float lowBound = _params.distanceBounds.lowValue();
  float highBound = _params.distanceBounds.highValue();
  float magnitude = _params.signedMagnitude();
  float dist = posDiff.length();
  if (dist < lowBound || dist > highBound) {
    return ofVec3f(0);
  }
  posDiff.normalize();
  float mag;
  float midDist = getInterpolated(lowBound, highBound, 0.5);
  if (dist < midDist) {
    mag = ofMap(dist,
                lowBound, midDist,
                0, magnitude,
                true);
  } else {
    mag = ofMap(dist,
                midDist, highBound,
                magnitude, 0,
                true);
  }
  return posDiff * mag;
}
