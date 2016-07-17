//
//  Context.cpp
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#include "Context.h"

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
