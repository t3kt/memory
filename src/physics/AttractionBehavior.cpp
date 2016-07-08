//
//  AttractionBehavior.cpp
//  memory
//
//  Created by tekt on 7/7/16.
//
//

#include "AttractionBehavior.h"
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include <ofMain.h>

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
  ofSetColor(ofFloatColor(0.9, 0.1, 0.1, 0.02));
  ofDrawSphere(entity->position(), _params.distanceBounds.lowValue());
  ofSetColor(ofFloatColor(0.1, 0.1, 0.9, 0.03));
  ofDrawSphere(entity->position(), _params.distanceBounds.highValue());
  ofPopStyle();
}

void AbstractAttractionBehavior::endDebugDraw() {
  ofPopStyle();
}
