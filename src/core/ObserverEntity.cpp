//
//  ObserverEntity.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#include <ofMain.h>
#include "../core/Info.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"
#include "../core/State.h"

ObserverEntity::ObserverEntity(ofVec3f pos, float life, const State& state)
: ParticleObject(pos)
, _totalLifetime(life)
, _lifeFraction(1)
, _startTime(state.time) {
}

void ObserverEntity::addOccurrence(std::shared_ptr<OccurrenceEntity> occurrence) {
  for (auto& other : occurrence->connectedObservers()) {
    if (other.first == id()) {
      continue;
    }
    addObserver(other.second);
  }
  _connectedOccurrences.add(occurrence);
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

void ObserverEntity::detachConnections() {
  for (auto& occurrence : _connectedOccurrences) {
    occurrence.second->removeObserver(id());
  }
  for (auto& observer : _connectedObservers) {
    observer.second->removeObserver(id());
  }
}

void ObserverEntity::outputFields(std::ostream &os) const {
  ParticleObject::outputFields(os);
  os << ", totalLifetime: " << _totalLifetime
      << ", lifeFraction: " << _lifeFraction
      << ", connectedOccurrences: " << _connectedOccurrences.size()
      << ", connectedObservers: " << _connectedObservers.size();
}

void ObserverEntity::fillInfo(Info& info) const {
  ParticleObject::fillInfo(info);
  info.add("lifeFraction:", _lifeFraction);
  info.add("totalLifeTime:", _totalLifetime);
  info.add("connObservers:", _connectedObservers.size());
  info.add("connOccurrences:", _connectedOccurrences.size());
}

void ObserverEntity::addSerializedFields(Json::object &obj,
                                         const SerializationContext& context) const {
  ParticleObject::addSerializedFields(obj, context);
  JsonUtil::mergeInto(obj, {
    {"startTime", _startTime - context.time()},
    {"totalLifetime", _totalLifetime},
    // omit lifetimeFraction since it's calculated
  });
}

void ObserverEntity::deserializeFields(const Json &obj,
                                       const SerializationContext &context) {
  ParticleObject::deserializeFields(obj, context);
  _startTime = JsonUtil::fromJson<float>(obj["startTime"]) + context.time();
  _totalLifetime = JsonUtil::fromJson<float>(obj["totalLifetime"]);
}

void ObserverEntity::addSerializedRefs(Json::object &obj,
                                       const SerializationContext &context) const {
  obj["connectedObservers"] = _connectedObservers.idsToJson();
  obj["connectedOccurrences"] = _connectedOccurrences.idsToJson();
}

void ObserverEntity::deserializeRefs(const Json &obj,
                                     SerializationContext &context) {
  if (obj.is_null()) {
    return;
  }
  JsonUtil::assertHasType(obj, Json::OBJECT);
  context.observers.loadDeserializedRefsInto(_connectedObservers, obj["connectedObservers"]);
  context.occurrences.loadDeserializedRefsInto(_connectedOccurrences, obj["connectedOccurrences"]);
}

void ObserverEntity::performActionOnConnected(ObjectPtrAction action) {
  for (auto& entity : _connectedObservers) {
    action(entity.second);
  }
  for (auto& entity : _connectedOccurrences) {
    action(entity.second);
  }
}
