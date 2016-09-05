//
//  Context.cpp
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#include "../core/Context.h"
#include "../navigation/NavigatorEntity.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"
#include "../core/ParticleObject.h"

template<>
ObjectManager<AnimationObject>&
Context::getEntities<AnimationObject>() {
  return animations;
}

template<>
const ObjectManager<AnimationObject>&
Context::getEntities<AnimationObject>() const {
  return animations;
}

template<>
ObjectManager<NavigatorEntity>&
Context::getEntities<NavigatorEntity>() {
  return navigators;
}

template<>
const ObjectManager<NavigatorEntity>&
Context::getEntities<NavigatorEntity>() const {
  return navigators;
}

template<>
ObjectManager<ObserverEntity>&
Context::getEntities<ObserverEntity>() {
  return observers;
}

template<>
const ObjectManager<ObserverEntity>&
Context::getEntities<ObserverEntity>() const {
  return observers;
}

template<>
ObjectManager<OccurrenceEntity>&
Context::getEntities<OccurrenceEntity>() {
  return occurrences;
}

template<>
const ObjectManager<OccurrenceEntity>&
Context::getEntities<OccurrenceEntity>() const {
  return occurrences;
}

void Context::performActionOnParticleEntityPtrs(std::function<void (std::shared_ptr<ParticleObject>)> action) {
  for (auto& entity : navigators) {
    action(entity);
  }
  for (auto& entity : observers) {
    action(entity);
  }
  for (auto& entity : occurrences) {
    action(entity);
  }
}



template<>
void Context::performEntityAction<WorldObject, WorldObject>(PtrRefAction<WorldObject> action) {
//  getEntities<AnimationObject>().performTypedAction(action);
}

void Context::clear() {
  animations.clear();
  navigators.clear();
  observers.clear();
  occurrences.clear();
}

Json Context::to_json() const {
  return Json::object {
    {"observers", observers.serializeEntities(*this)},
    {"occurrences", occurrences.serializeEntities(*this)},
  };
}

void Context::read_json(const Json& val) {
  JsonUtil::assertHasType(val, Json::OBJECT);
  Json observersArr = val["observers"];
  Json occurrencesArr = val["occurrences"];
  clear();
  observers.deserializeEntityFields(observersArr, *this);
  occurrences.deserializeEntityFields(occurrencesArr, *this);
  observers.deserializeEntityRefs(observersArr, *this);
  occurrences.deserializeEntityRefs(occurrencesArr, *this);
}
