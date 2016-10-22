//
//  EntityMap.h
//  memory
//
//  Created by tekt on 8/8/16.
//
//

#pragma once

#include <iterator>
#include <memory>
#include <ofMath.h>
#include <ofUtils.h>
#include <unordered_map>
#include "../core/Common.h"
#include "../core/JsonIO.h"
#include "../core/WorldObject.h"

// A map of references to entities, keyed by entity ID.
template<typename E>
class EntityMap
: public NonCopyable {
public:
  using EntityPtr = std::shared_ptr<E>;
  using Storage = std::unordered_map<ObjectId, EntityPtr>;
  using iterator = MapToValueIterator<ObjectId, EntityPtr>;
  using const_iterator = ConstMapToValueIterator<ObjectId, EntityPtr>;

  bool add(EntityPtr entity) {
    auto result = _map.insert(std::make_pair(entity->id(), entity));
    return result.second;
  }

  EntityPtr getAtIndex(std::size_t index) {
    if (index >= size()) {
      return EntityPtr();
    }
    auto iter = std::next(_map.begin(), index);
    return iter == _map.end() ? nullptr : iter->second;
  }

  EntityPtr operator[](ObjectId id) {
    auto iter = _map.find(id);
    return iter == _map.end() ? nullptr : iter->second;
  }

  bool containsId(ObjectId id) const { return _map.find(id) != end(); }

  void clear() { _map.clear(); }
  std::size_t erase(ObjectId id) { return _map.erase(id); }
  std::size_t size() const { return _map.size(); }
  bool empty() const { return _map.empty(); }
  iterator begin() { return _map.begin(); }
  iterator end() { return _map.end(); }
  const_iterator begin() const { return _map.begin(); }
  const_iterator end() const { return _map.end(); }

  EntityPtr getFirst() {
    if (empty()) {
      return EntityPtr();
    }
    return _map. begin()->second;
  }

  // Perform an action on each entity in the map.
  template<typename A>
  void performAction(A action) {
    for (auto& entry : _map) {
      action(entry.second);
    }
  }

  Json idsToJson() const {
    Json::array arr;
    for (const auto& entity : _map) {
      int idVal = static_cast<int>(entity.second->id());
      arr.push_back(idVal);
    }
    return arr;
  }

  template<typename T>
  std::shared_ptr<T> getFirstOfType() {
    for (auto& entry : _map) {
      if (entry.second->entityType() == T::type) {
        return std::dynamic_pointer_cast<T>(entry.second);
      }
    }
    return std::shared_ptr<T>();
  }

  // Remove references to dead entities from the map.
  void cullDeadObjects() {
    for(auto iter = _map.begin();
        iter != _map.end();) {
      if (iter->second->alive()) {
        iter++;
      } else {
        iter = _map.erase(iter);
      }
    }
  }

  EntityPtr getRandomEntity() {
    if (empty()) {
      return nullptr;
    }
    auto index = static_cast<int>(ofRandom(0, size() - 1));
    return getAtIndex(index);
  }
private:
  Storage _map;
};
