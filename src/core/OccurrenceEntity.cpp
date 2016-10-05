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
      << ", occurrenceConnections: " << _occurrenceConnections.size()
      << ", observerConnections: " << _observerConnections.size();
}

void OccurrenceEntity::fillInfo(Info& info) const {
  ParticleObject::fillInfo(info);
  info.add("origRadius:", _originalRadius);
  info.add("actualRadius:", _actualRadius);
  info.add("amountOfObs:", _amountOfObservation);
  info.add("connOccurrences:", _occurrenceConnections.size());
  info.add("connObservers:", _observerConnections.size());
}

void OccurrenceEntity::detachConnections() {
  for (auto& connection : _occurrenceConnections) {
    connection->entity()->removeOccurrence(id());
  }
  for (auto& connection  : _observerConnections) {
    connection->entity()->removeObserver(id());
  }
}

void OccurrenceEntity::update(const State& state) {
  if (!hasConnectedObservers()) {
    kill();
    setAmountOfObservation(0);
  } else {
    float amount = 0;
    float radius = 0;
    for (const auto& connection : _observerConnections) {
      const auto& observer = connection->entity();
      amount += observer->getRemainingLifetimeFraction();
      float dist = position().distance(observer->position());
      if (dist > radius) {
        radius = dist;
      }
    }
    setAmountOfObservation(amount);
    setActualRadius(radius);
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
  obj["observerConnections"] = _observerConnections.to_json();
  obj["occurrenceConnections"] = _occurrenceConnections.to_json();
}

void OccurrenceEntity::deserializeRefs(const Json &obj,
                                       SerializationContext &context) {
  if (obj.is_null()) {
    return;
  }
  JsonUtil::assertHasType(obj, Json::OBJECT);
  context.observers.loadDeserializedRefsInto(_observerConnections, obj["observerConnections"]);
  context.occurrences.loadDeserializedRefsInto(_occurrenceConnections, obj["occurrenceConnections"]);
}

void OccurrenceEntity::performActionOnConnected(ObjectPtrAction action) {
  _observerConnections.performAction(action);
  _occurrenceConnections.performAction(action);
}
