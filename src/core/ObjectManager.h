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
    for (auto i = std::begin(_objects);
         i != std::end(_objects);) {
      shared_ptr<T>& object = i->second;
      if (object->alive()) {
        i++;
      } else {
        deadObjects.push_back(object);
        i = _objects.erase(i);
      }
    }
    return deadObjects;
  }
  
  void draw(const State& state) {
    for (auto entry : _objects) {
      entry.second->draw(state);
    }
  }
  
  void add(shared_ptr<T> object) {
    _objects.insert(std::make_pair(object->id, object));
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
