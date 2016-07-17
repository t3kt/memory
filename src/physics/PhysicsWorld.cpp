//
//  PhysicsWorld.cpp
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include "ParticleObject.h"
#include "PhysicsWorld.h"

template<>
ObjectManager<ObserverEntity>& PhysicsWorld::getEntities<ObserverEntity>() {
  return observers();
}

template<>
const ObjectManager<ObserverEntity>& PhysicsWorld::getEntities<ObserverEntity>() const {
  return observers();
}

template<>
ObjectManager<OccurrenceEntity>& PhysicsWorld::getEntities<OccurrenceEntity>() {
  return occurrences();
}

template<>
const ObjectManager<OccurrenceEntity>& PhysicsWorld::getEntities<OccurrenceEntity>() const {
  return occurrences();
}

void PhysicsWorld::performActionOnAllEntities(EntityAction action) {
  observers().performTypedAction(action);
  occurrences().performTypedAction(action);
};
