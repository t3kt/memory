//
//  OccurrenceEntity.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#include "OccurrenceEntity.h"
#include "ObserverEntity.h"
#include <ofMain.h>

OccurrenceEntity::OccurrenceEntity(ofVec3f pos, float radius, const State& state)
: ParticleObject(pos)
, _actualRadius(0)
, _originalRadius(radius)
, _startTime(state.time)
, _amountOfObservation(0) {}

void OccurrenceEntity::outputFields(std::ostream &os) const {
  ParticleObject::outputFields(os);
  os << ", originalRadius: " << _originalRadius
      << ", actualRadius: " << _actualRadius
      << ", connectedOccurrences: " << _connectedOccurrences.size()
      << ", connectedObservers: " << _connectedObservers.size();
}

void OccurrenceEntity::detachConnections() {
  for (auto& occurrence : _connectedOccurrences) {
    occurrence.second->removeOccurrence(id());
  }
  for (auto& observer : _connectedObservers) {
    observer.second->removeObserver(id());
  }
}
