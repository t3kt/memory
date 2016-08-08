//
//  WorldObject.h
//  memory-prototype-2
//
//  Created by tekt on 6/24/16.
//
//

#ifndef WorldObject_h
#define WorldObject_h

#include <iostream>
#include <iterator>
#include <memory>
#include <ofMath.h>
#include <ofUtils.h>
#include <unordered_map>
#include "Common.h"
#include "JsonIO.h"
#include "Serialization.h"

typedef int ObjectId;

const ObjectId NO_OBJECT_ID = -1;

enum class EntityType {
  ANIMATION,
  NAVIGATOR,
  OBSERVER,
  OCCURRENCE,
};

template<typename T>
EntityType getEntityType();

class WorldObject
: public Outputable
, public Serializable {
public:
  WorldObject();
  virtual ~WorldObject() {}
  
  const ObjectId& id() const { return _id; }

  bool alive() const { return _alive; }

  void kill() { _alive = false; }
  
  virtual bool visible() const { return this->alive() && _alpha > 0; }

  float alpha() const { return _alpha; }
  void setAlpha(float alpha) { _alpha = alpha; }

  virtual EntityType entityType() const = 0;

  virtual void deserializeFields(const Json& obj,
                                 const SerializationContext& context) override;

protected:
  virtual std::string typeName() const override;
  virtual void outputFields(std::ostream& os) const override;
  virtual void addSerializedFields(Json::object& obj,
                                   const SerializationContext& context) const override;
private:
  float _alpha;
  bool _alive;
  ObjectId _id;
};

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

#endif /* WorldObject_h */
