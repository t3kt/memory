//
//  OccurrenceEntity.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#include <ofMain.h>
#include "../core/Context.h"
#include "../core/Info.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"

OccurrenceEntity::OccurrenceEntity(ofVec3f pos,
                                   float radius,
                                   float radiusFraction,
                                   const State& state)
: ParticleObject(pos)
, _actualRadius(0)
, _originalRadius(radius)
, _originalRadiusFraction(radiusFraction)
, _startTime(state.time)
, _amountOfObservation(0) {}

void OccurrenceEntity::outputFields(std::ostream &os) const {
  ParticleObject::outputFields(os);
  os << ", originalRadius: " << _originalRadius
      << ", actualRadius: " << _actualRadius
      << ", connectedOccurrences: " << _connectedOccurrences.size()
      << ", connectedObservers: " << _connectedObservers.size();
}

void OccurrenceEntity::fillInfo(Info& info) const {
  ParticleObject::fillInfo(info);
  info.add("origRadius:", _originalRadius);
  info.add("actualRadius:", _actualRadius);
  info.add("amountOfObs:", _amountOfObservation);
  info.add("connObservers:", _connectedObservers.size());
  info.add("connOccurrences:", _connectedOccurrences.size());
}

void OccurrenceEntity::detachConnections() {
  for (auto& occurrence : _connectedOccurrences) {
    occurrence.second->removeOccurrence(id());
  }
  for (auto& observer : _connectedObservers) {
    observer.second->removeObserver(id());
  }
}

void OccurrenceEntity::addSerializedFields(Json::object &obj,
                                           const SerializationContext &context) const {
  ParticleObject::addSerializedFields(obj, context);
  JsonUtil::mergeInto(obj, {
    {"originalRadius", _originalRadius},
    {"originalRadiusFraction", _originalRadiusFraction},
    // omit actualRadius since it's calculated
    {"startTime", _startTime - context.time()},
    // omit amountOfObservation since it's calculated
  });
}

void OccurrenceEntity::deserializeFields(const Json &obj,
                                         const SerializationContext &context) {
  ParticleObject::deserializeFields(obj, context);
  _originalRadius = JsonUtil::fromJson<float>(obj["originalRadius"]);
  _originalRadiusFraction = JsonUtil::fromJson<float>(obj["originalRadiusFraction"]);
  _startTime = JsonUtil::fromJson<float>(obj["startTime"]) + context.time();
}

void OccurrenceEntity::addSerializedRefs(Json::object &obj,
                                         const SerializationContext &context) const {
  obj["connectedObservers"] = _connectedObservers.idsToJson();
  obj["connectedOccurrences"] = _connectedOccurrences.idsToJson();
}

void OccurrenceEntity::deserializeRefs(const Json &obj,
                                       SerializationContext &context) {
  if (obj.is_null()) {
    return;
  }
  JsonUtil::assertHasType(obj, Json::OBJECT);
  context.observers.loadDeserializedRefsInto(_connectedObservers, obj["connectedObservers"]);
  context.occurrences.loadDeserializedRefsInto(_connectedOccurrences, obj["connectedOccurrences"]);
}

void OccurrenceEntity::performActionOnConnected(ObjectPtrRefAction action) {
  for (auto& entity : _connectedObservers) {
    action(entity.second);
  }
  for (auto& entity : _connectedOccurrences) {
    action(entity.second);
  }
}
