//
//  Context.cpp
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#include "Context.h"
#include "NavigatorEntity.h"
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include "ParticleObject.h"

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
