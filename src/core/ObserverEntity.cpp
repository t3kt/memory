//
//  ObserverEntity.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include "State.h"
#include <ofMain.h>

ObserverEntity::ObserverEntity(ofVec3f pos, float life, const State& state)
: ParticleObject(pos, state)
, _totalLifetime(life)
, _lifeFraction(1) {
}

void ObserverEntity::addOccurrence(shared_ptr<OccurrenceEntity> occurrence) {
  ofLogVerbose() << "BEGIN Observer[" << id << "]: adding Occurrence[" << occurrence->id << "]";
  for (auto& other : occurrence->connectedObservers()) {
    if (other.first == id) {
      continue;
    }
    addObserver(other.second);
  }
  _connectedOccurrences.add(occurrence);
  ofLogVerbose() << "  after adds: " << *this;
  ofLogVerbose() << "END Observer[" << id << "]: adding Occurrence[" << occurrence->id << "]";
}

void ObserverEntity::update(const State &state) {
  float elapsed = state.time - _startTime;
  if (elapsed > _totalLifetime) {
    _lifeFraction = 0.0f;
    kill();
  } else {
    _lifeFraction = ofMap(elapsed, 0.0f, _totalLifetime, 1.0f, 0.0f);
  }
}

void ObserverEntity::handleDeath() {
  ofLogNotice() << "Observer died: " << *this;
  for (auto& occurrence : _connectedOccurrences) {
    occurrence.second->removeObserver(id);
  }
  for (auto& observer : _connectedObservers) {
    if (!observer.second) {
      continue;
    }
    observer.second->removeObserver(id);
  }
}

void ObserverEntity::outputFields(std::ostream &os) const {
  ParticleObject::outputFields(os);
  os << ", totalLifetime: " << _totalLifetime
      << ", lifeFraction: " << _lifeFraction
      << ", connectedOccurrences: " << _connectedOccurrences.size()
      << ", connectedObservers: " << _connectedObservers.size();
}
