//
//  EntityMap.h
//  memory
//
//  Created by tekt on 8/8/16.
//
//

#ifndef EntityMap_h
#define EntityMap_h

#include <iterator>
#include <memory>
#include <ofMath.h>
#include <ofUtils.h>
#include <unordered_map>
#include "../core/Common.h"
#include "../core/JsonIO.h"
#include "../core/WorldObject.h"

template<typename E>
class EntityMap {
public:
  using EntityPtr = std::shared_ptr<E>;
  using Storage = std::unordered_map<ObjectId, EntityPtr>;
  using iterator = typename Storage::iterator;
  using const_iterator = typename Storage::const_iterator;

  void add(EntityPtr entity) { _map[entity->id()] = entity; }

  EntityPtr getAtIndex(std::size_t index) {
    if (index >= size()) {
      return EntityPtr();
    }
    auto iter = std::next(begin(), index);
    if (iter != end()) {
      return iter->second;
    }
    return EntityPtr();
  }

  EntityPtr operator[](ObjectId id) {
    auto iter = _map.find(id);
    if (iter == end()) {
      return EntityPtr();
    }
    return iter->second;
  }

  bool containsId(ObjectId id) const {
    auto iter = _map.find(id);
    return iter != end();
  }

  void clear() { _map.clear(); }

  std::size_t erase(ObjectId id) { return _map.erase(id); }

  std::size_t size() const { return _map.size(); }

  bool empty() const { return _map.empty(); }

  iterator begin() { return _map.begin(); }
  iterator end() { return _map.end(); }
  const_iterator begin() const { return _map.begin(); }
  const_iterator end() const { return _map.end(); }

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
private:
  Storage _map;
};

template<typename E>
std::shared_ptr<E> getRandomEntity(EntityMap<E>& entities) {
  if (entities.empty()) {
    return std::shared_ptr<E>();
  }
  auto index = static_cast<int>(ofRandom(0, entities.size() - 1));
  return entities.getAtIndex(index);
}

#endif /* EntityMap_h */
