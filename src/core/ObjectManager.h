//
//  ObjectManager.h
//  memory-prototype-2
//
//  Created by tekt on 6/23/16.
//
//

#ifndef ObjectManager_h
#define ObjectManager_h

#include <map>
#include <memory>
#include <vector>
#include <list>
#include <functional>
#include "WorldObject.h"
#include "State.h"
#include "Events.h"

template <class T>
class ObjectManager {
public:
  using StorageList = std::list<std::shared_ptr<T>>;
  
  void update(const State& state) {
    for (auto entity : _objects) {
      entity->update(state);
    }
  }
  
  std::vector<std::shared_ptr<T>> extractDeadObjects() {
    std::vector<std::shared_ptr<T>> deadObjects;
    cullDeadObjects([&](std::shared_ptr<T> entity) {
      deadObjects.push_back(entity);
    });
    return deadObjects;
  }
  
  void cullDeadObjects(std::function<void(shared_ptr<T>)> callback) {
    for (auto i = std::begin(_objects);
         i != std::end(_objects);) {
      shared_ptr<T>& object = *i;
      if (object->alive()) {
        i++;
      } else {
        callback(object);
        i = _objects.erase(i);
      }
    }
  }
  
  void draw(const State& state) {
    for (auto entity : _objects) {
      if (entity->visible()) {
        entity->draw(state);
      }
    }
  }
  
  void add(shared_ptr<T> object) {
    _objects.push_back(object);
  }

  template<typename ...Args>
  void add(shared_ptr<T> object, Args&... others) {
    add(object);
    add(others...);
  }
  
  bool eraseById(ObjectId id) {
    for (auto i = std::begin(_objects);
         i != std::end(_objects);
         i++) {
      if ((*i)->id == id) {
        _objects.erase(i);
        return true;
      }
    }
    return false;
  }
  
  void performAction(std::function<void(shared_ptr<T>)> action) {
    for (auto entity : _objects) {
      action(entity);
    }
  }
  
  std::size_t size() const {
    return _objects.size();
  }
  
  bool isEmpty() const {
    return _objects.empty();
  }

  typename StorageList::iterator begin() {
    return _objects.begin();
  }

  typename StorageList::iterator end() {
    return _objects.end();
  }

private:
  StorageList _objects;
};

#endif /* ObjectManager_h */
