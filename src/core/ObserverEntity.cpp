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

ObserverEntity::ObserverEntity(ofVec3f pos, float decay, const State& state)
: ParticleObject(pos)
, _decayRate(decay)
, _lifeFraction(1)
, _startTime(state.time) {
}

void ObserverEntity::addOccurrence(std::shared_ptr<OccurrenceEntity> occurrence) {
  for (auto& other : occurrence->getConnectedObservers()) {
    if (other.first == id()) {
      continue;
    }
    addObserver(other.second);
  }
  _connectedOccurrences.add(occurrence);
}

void ObserverEntity::update(const State &state) {
  _lifeFraction -= _decayRate * state.timeDelta;
  if (_lifeFraction <= 0) {
    _lifeFraction = 0;
    kill();
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
  os << ", lifeFraction: " << _lifeFraction
      << ", decayRate: " << _decayRate
      << ", sick: " << _sick
      << ", connectedOccurrences: " << _connectedOccurrences.size()
      << ", connectedObservers: " << _connectedObservers.size();
}

void ObserverEntity::fillInfo(Info& info) const {
  ParticleObject::fillInfo(info);
  info.add("lifeFraction:", _lifeFraction);
  info.add("decayRate:", _decayRate);
  info.add("connObservers:", _connectedObservers.size());
  info.add("connOccurrences:", _connectedOccurrences.size());
  if (_sick) {
    info.add("sick:", _sick);
  }
}

void ObserverEntity::addSerializedFields(Json::object &obj,
                                         const SerializationContext& context) const {
  ParticleObject::addSerializedFields(obj, context);
  JsonUtil::mergeInto(obj, {
    {"startTime", _startTime - context.time()},
    {"lifeFraction", _lifeFraction},
    {"decayRate", _decayRate},
    {"sick", _sick},
  });
}

void ObserverEntity::deserializeFields(const Json &obj,
                                       const SerializationContext &context) {
  ParticleObject::deserializeFields(obj, context);
  _startTime = JsonUtil::fromJson<float>(obj["startTime"]) + context.time();
  _decayRate = JsonUtil::fromJson<float>(obj["decayRate"]);
  _lifeFraction = JsonUtil::fromJson<float>(obj["lifeFraction"]);
  _sick = JsonUtil::fromJson<bool>(obj["sick"]);
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

void ObserverEntity::performActionOnConnected(ObjectPtrRefAction action) {
  for (auto& entity : _connectedObservers) {
    action(entity.second);
  }
  for (auto& entity : _connectedOccurrences) {
    action(entity.second);
  }
}
