//
//  PhysicsWorld.cpp
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#include "PhysicsWorld.h"
#include "ObserversController.h"
#include "OccurrencesController.h"

ObjectManager<ObserverEntity>& PhysicsWorld::observers() {
  return _observers.entities();
}

const ObjectManager<ObserverEntity>& PhysicsWorld::observers() const {
  return _observers.entities();
}

ObjectManager<OccurrenceEntity>& PhysicsWorld::occurrences() {
  return _occurrences.entities();
}

const ObjectManager<OccurrenceEntity>& PhysicsWorld::occurrences() const {
  return _occurrences.entities();
}

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
