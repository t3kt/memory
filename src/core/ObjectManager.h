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
#include <functional>
#include "WorldObject.h"
#include "State.h"
#include "Events.h"

template <class T>
class ObjectManager {
public:
  using StorageType = std::map<ObjectId, std::shared_ptr<T>>;
  
  void update(const State& state) {
    for (auto entry : _objects) {
      entry.second->update(state);
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
      shared_ptr<T>& object = i->second;
      if (object->alive()) {
        i++;
      } else {
        callback(object);
        i = _objects.erase(i);
      }
    }
  }
  
  void draw(const State& state) {
    for (auto entry : _objects) {
      if (entry.second->visible()) {
        entry.second->draw(state);
      }
    }
  }
  
  void add(shared_ptr<T> object) {
    _objects.insert(std::make_pair(object->id, object));
  }

  template<typename ...Args>
  void add(shared_ptr<T> object, Args&... others) {
    add(object);
    add(others...);
  }
  
  bool eraseById(ObjectId id) {
    auto i = _objects.find(id);
    if (i == std::end(_objects)) {
      return false;
    } else {
      i->second.reset();
      _objects.erase(i);
      return true;
    }
  }
  
  void performAction(std::function<void(shared_ptr<T>)> action) {
    for (auto entry : _objects) {
      action(entry.second);
    }
  }
  
  std::size_t size() const {
    return _objects.size();
  }
  
  bool isEmpty() const {
    return _objects.empty();
  }
  
private:
  StorageType _objects;
};

#endif /* ObjectManager_h */
