//
//  ObserverEntity.cpp
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

std::shared_ptr<ObserverEntity>
ObserverEntity::createEmpty(const Context& context) {
  return std::shared_ptr<ObserverEntity>(new ObserverEntity(context.entityState));
}

ObserverEntity::ObserverEntity(ofVec3f pos,
                               float decay,
                               const ClockState& state)
: ParticleObject(pos, state)
, _decayRate(decay)
, _lifeFraction(1) {
}

void ObserverEntity::addOccurrence(std::shared_ptr<OccurrenceEntity> occurrence) {
  for (auto& connection : occurrence->getObserverConnections()) {
    auto& other = connection->entity();
    if (other->id() == id()) {
      continue;
    }
    addObserver(other);
  }
  _occurrenceConnections.getOrAdd(shared_from_this(),
                                  occurrence);
}

void ObserverEntity::addObserver(std::shared_ptr<ObserverEntity> observer) {
  _observerConnections.getOrAdd(shared_from_this(),
                                observer);
}

void ObserverEntity::update(const ClockState &state) {
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

void ObserverEntity::addSerializedFields(ofJson &obj,
                                         const SerializationContext& context) const {
  ParticleObject::addSerializedFields(obj, context);
  ofxTCommon::JsonUtil::mergeInto(obj, {
    {"lifeFraction", _lifeFraction},
    {"decayRate", _decayRate},
    {"sick", _sick},
  });
}

void ObserverEntity::deserializeFields(const ofJson &obj,
                                       const SerializationContext &context) {
  ParticleObject::deserializeFields(obj, context);
  _decayRate = ofxTCommon::JsonUtil::fromJson<float>(obj["decayRate"]);
  _lifeFraction = ofxTCommon::JsonUtil::fromJson<float>(obj["lifeFraction"]);
  _sick = ofxTCommon::JsonUtil::fromJson<bool>(obj["sick"]);
}

void ObserverEntity::addSerializedRefs(ofJson &obj,
                                       const SerializationContext &context) const {
  obj["observerConnections"] = _observerConnections.toJson();
  obj["occurrenceConnections"] = _occurrenceConnections.toJson();
}

void ObserverEntity::deserializeRefs(const ofJson &obj,
                                     SerializationContext &context) {
  if (obj.is_null()) {
    return;
  }
  ofxTCommon::JsonUtil::assertIsObject(obj);
  context.observers.loadDeserializedRefsInto(shared_from_this(),
                                             _observerConnections,
                                             obj["observerConnections"]);
  context.occurrences.loadDeserializedRefsInto(shared_from_this(),
                                               _occurrenceConnections,
                                               obj["occurrenceConnections"]);
}

void ObserverEntity::performActionOnConnected(ObjectPtrAction action) {
  _observerConnections.performAction(action);
  _occurrenceConnections.performAction(action);
}
