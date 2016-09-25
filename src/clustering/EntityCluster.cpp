//
//  EntityCluster.cpp
//  memory
//
//  Created by tekt on 9/24/16.
//
//

#include "../clustering/EntityCluster.h"
#include "../core/NodeEntity.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"
#include "../core/ParticleObject.h"

template<>
EntityMap<NodeEntity>&
EntityCluster::getEntities() { return _nodes; }

template<>
EntityMap<ObserverEntity>&
EntityCluster::getEntities() { return _observers; }

template<>
EntityMap<OccurrenceEntity>&
EntityCluster::getEntities() { return _occurrences; }

EntityCluster::EntityCluster()
: _id(ObjectIds::next()) { }

void EntityCluster::performAllEntityAction(PtrAction<ParticleObject> action) {
  performEntityAction<NodeEntity>([&](std::shared_ptr<NodeEntity>& entity) {
    action(std::dynamic_pointer_cast<ParticleObject>(entity));
  });
  performEntityAction<ObserverEntity>([&](std::shared_ptr<ObserverEntity>& entity) {
    action(std::dynamic_pointer_cast<ParticleObject>(entity));
  });
  performEntityAction<OccurrenceEntity>([&](std::shared_ptr<OccurrenceEntity>& entity) {
    action(std::dynamic_pointer_cast<ParticleObject>(entity));
  });
}

void EntityCluster::cullDeadObjects() {
  _nodes.cullDeadObjects();
  _observers.cullDeadObjects();
  _occurrences.cullDeadObjects();
}
