//
//  EntityManager.h
//  memory-prototype-2
//
//  Created by tekt on 6/23/16.
//
//

#ifndef EntityManager_h
#define EntityManager_h

#include <map>
#include <memory>
#include <functional>
#include "Entity.h"
#include "State.h"

template <class E>
class EntityManager {
public:
  
  void update(const State& state) {
    for (auto i = std::begin(_entities);
         i != std::end(_entities);) {
      shared_ptr<E>& entity = i->second;
      entity->update(state);
      if (entity->isAlive(state)) {
        i++;
      } else {
        entity->handleDeath();
        i = _entities.erase(i);
      }
    }
  }
  
  void draw(const State& state) {
    for (auto entry : _entities) {
      entry.second->draw(state);
    }
  }
  
  void addEntity(shared_ptr<E> entity) {
    _entities.insert(std::make_pair(entity->id, entity));
  }
  
  void performAction(std::function<void(shared_ptr<E>)> action) {
    for (auto entry : _entities) {
      action(entry.second);
    }
  }
  
private:
  std::map<EntityId, shared_ptr<E>> _entities;
};

#endif /* EntityManager_h */
