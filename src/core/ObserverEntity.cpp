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
  for (auto& connection : occurrence->getObserverConnections()) {
    auto& other = connection->entity();
    if (other->id() == id()) {
      continue;
    }
    addObserver(other);
  }
  auto connection =
  std::make_shared<EntityConnection<OccurrenceEntity>>(occurrence);
  _occurrenceConnections.addConnection(connection);
}

void ObserverEntity::addObserver(std::shared_ptr<ObserverEntity> observer) {
  auto connection =
  std::make_shared<EntityConnection<ObserverEntity>>(observer);
  _observerConnections.addConnection(connection);
}

void ObserverEntity::update(const State &state) {
  _lifeFraction -= _decayRate * state.timeDelta;
  if (_lifeFraction <= 0) {
    _lifeFraction = 0;
    kill();
  }
}

void ObserverEntity::detachConnections() {
  for (auto& connection : _occurrenceConnections) {
    connection->entity()->removeObserver(id());
  }
  for (auto& connection : _observerConnections) {
    connection->entity()->removeObserver(id());
  }
}

void ObserverEntity::outputFields(std::ostream &os) const {
  ParticleObject::outputFields(os);
  os << ", lifeFraction: " << _lifeFraction
      << ", decayRate: " << _decayRate
      << ", sick: " << _sick
      << ", occurrenceConnections: " << _occurrenceConnections.size()
      << ", observerConnections: " << _observerConnections.size();
}

void ObserverEntity::fillInfo(Info& info) const {
  ParticleObject::fillInfo(info);
  info.add("lifeFraction:", _lifeFraction);
  info.add("decayRate:", _decayRate);
  info.add("connOccurrences:", _occurrenceConnections.size());
  info.add("connObservers:", _observerConnections.size());
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
  obj["observerConnections"] = _observerConnections.to_json();
  obj["occurrenceConnections"] = _occurrenceConnections.to_json();
}

void ObserverEntity::deserializeRefs(const Json &obj,
                                     SerializationContext &context) {
  if (obj.is_null()) {
    return;
  }
  JsonUtil::assertHasType(obj, Json::OBJECT);
  context.observers.loadDeserializedRefsInto(_observerConnections, obj["observerConnections"]);
  context.occurrences.loadDeserializedRefsInto(_occurrenceConnections, obj["occurrenceConnections"]);
}

void ObserverEntity::performActionOnConnected(ObjectPtrAction action) {
  _observerConnections.performAction(action);
  _occurrenceConnections.performAction(action);
}
