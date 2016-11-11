//
//  Context.cpp
//

#include "../core/Context.h"
#include "../core/NodeEntity.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"
#include "../core/ParticleObject.h"
#include "../navigation/NavigatorEntity.h"

template<>
ObjectManager<AnimationObject>&
Context::getEntities<AnimationObject>() { return animations; }

template<>
const ObjectManager<AnimationObject>&
Context::getEntities<AnimationObject>() const { return animations; }

template<>
ObjectManager<NavigatorEntity>&
Context::getEntities<NavigatorEntity>() { return navigators; }

template<>
const ObjectManager<NavigatorEntity>&
Context::getEntities<NavigatorEntity>() const { return navigators; }

template<>
ObjectManager<NodeEntity>&
Context::getEntities<NodeEntity>() { return nodes; }

template<>
const ObjectManager<NodeEntity>&
Context::getEntities<NodeEntity>() const { return nodes; }

template<>
ObjectManager<ObserverEntity>&
Context::getEntities<ObserverEntity>() { return observers; }

template<>
const ObjectManager<ObserverEntity>&
Context::getEntities<ObserverEntity>() const { return observers; }

template<>
ObjectManager<OccurrenceEntity>&
Context::getEntities<OccurrenceEntity>() { return occurrences; }

template<>
const ObjectManager<OccurrenceEntity>&
Context::getEntities<OccurrenceEntity>() const { return occurrences; }

void Context::performActionOnParticleEntityPtrs(std::function<void (std::shared_ptr<ParticleObject>)> action) {
  for (auto& entity : navigators) {
    action(entity);
  }
  for (auto& entity : nodes) {
    action(entity);
  }
  for (auto& entity : observers) {
    action(entity);
  }
  for (auto& entity : occurrences) {
    action(entity);
  }
}

void Context::clear() {
  animations.clear();
  navigators.clear();
  nodes.clear();
  observers.clear();
  occurrences.clear();
}

ofJson Context::toJson() const {
  return {
    {"observers", observers.serializeEntities(*this)},
    {"occurrences", occurrences.serializeEntities(*this)},
  };
}

void Context::readJson(const ofJson& val) {
  ofxTCommon::JsonUtil::assertIsObject(val);
  const auto& observersArr = val["observers"];
  const auto& occurrencesArr = val["occurrences"];
  clear();
  observers.deserializeEntityFields(observersArr, *this);
  occurrences.deserializeEntityFields(occurrencesArr, *this);
  observers.deserializeEntityRefs(observersArr, *this);
  occurrences.deserializeEntityRefs(occurrencesArr, *this);
}
