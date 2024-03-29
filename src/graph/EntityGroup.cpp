//
//  EntityGroup.cpp
//  memory
//
//  Created by tekt on 9/24/16.
//
//

#include "../graph/EntityGroup.h"
#include "../core/NodeEntity.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"

template<>
EntityMap<NodeEntity>&
EntityGroup::getEntities() { return _nodes; }

template<>
EntityMap<ObserverEntity>&
EntityGroup::getEntities() { return _observers; }

template<>
EntityMap<OccurrenceEntity>&
EntityGroup::getEntities() { return _occurrences; }

EntityGroup::EntityGroup()
: _id(ObjectIds::next()) { }

void EntityGroup::performAllEntityAction(PtrAction<ParticleObject> action) {
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

void EntityGroup::cullDeadObjects() {
  _nodes.cullDeadObjects();
  _observers.cullDeadObjects();
  _occurrences.cullDeadObjects();
}

void EntityGroup::clear() {
  _nodes.clear();
  _observers.clear();
  _occurrences.clear();
}

//template<>
bool EntityGroup::add(std::shared_ptr<WorldObject> entity) {
  auto type = entity->entityType();
  switch (type) {
    case EntityType::NODE:
      return getEntities<NodeEntity>().add(std::dynamic_pointer_cast<NodeEntity>(entity));
//      return add(std::dynamic_pointer_cast<NodeEntity>(entity));
    case EntityType::OBSERVER:
      return getEntities<ObserverEntity>().add(std::dynamic_pointer_cast<ObserverEntity>(entity));
//      return add(std::dynamic_pointer_cast<ObserverEntity>(entity));
    case EntityType::OCCURRENCE:
      return getEntities<OccurrenceEntity>().add(std::dynamic_pointer_cast<OccurrenceEntity>(entity));
//      return add(std::dynamic_pointer_cast<OccurrenceEntity>(entity));
    default:
      return false;
  }
}

//template<>
bool EntityGroup::add(std::shared_ptr<ParticleObject> entity) {
  auto type = entity->entityType();
  switch (type) {
    case EntityType::NODE:
      return getEntities<NodeEntity>().add(std::dynamic_pointer_cast<NodeEntity>(entity));
//      return add(std::dynamic_pointer_cast<NodeEntity>(entity));
    case EntityType::OBSERVER:
      return getEntities<ObserverEntity>().add(std::dynamic_pointer_cast<ObserverEntity>(entity));
//      return add(std::dynamic_pointer_cast<ObserverEntity>(entity));
    case EntityType::OCCURRENCE:
      return getEntities<OccurrenceEntity>().add(std::dynamic_pointer_cast<OccurrenceEntity>(entity));
//      return add(std::dynamic_pointer_cast<OccurrenceEntity>(entity));
    default:
      return false;
  }
}

void EntityGroup::addConnections(ParticlePtr fromEntity,
                                 std::size_t depthLimit) {
  bool descendants = depthLimit == -1 || depthLimit > 0;
  fromEntity->performActionOnConnected([&](std::shared_ptr<WorldObject> entity) {
    auto type = entity->entityType();
    //
  });
  //...
  //...
}
