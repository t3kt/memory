//
//  Connection.h
//  memory
//
//  Created by tekt on 10/3/16.
//
//

#pragma once

#include <memory>
#include <ofMath.h>
#include <unordered_map>
#include <type_traits>
#include "../core/Common.h"
#include "../core/JsonIO.h"
#include "../core/ParticleObject.h"
#include "../core/WorldObject.h"

// Base class for a connection to an entity (generally from
// another entity).
class AbstractConnection
: public NonCopyable
, public JsonWritable {
public:
  bool alive() const {
    return getEntityRef().id();
  }
  ObjectId entityId() const {
    return getEntityRef().id();
  }
protected:
  virtual ParticleObject& getEntityRef() = 0;
  virtual const ParticleObject& getEntityRef() const = 0;
};

// A connection to an entity of type E.
template<typename E>
class EntityConnection
: public AbstractConnection {
public:
  using EntityT = E;
  using EntityPtr = std::shared_ptr<E>;

  explicit EntityConnection(EntityPtr entity)
  : _entity(entity) { }

  EntityPtr& entity() { return _entity; }
  const EntityPtr& entity() const { return _entity; }

  Json to_json() const override {
    return Json::object {
      {"id", entityId()},
    };
  }
protected:
  ParticleObject& getEntityRef() override {
    return *_entity;
  }
  const ParticleObject& getEntityRef() const override {
    return *_entity;
  }
private:
  EntityPtr _entity;
};

// A map of connections to various entities.
// TConn is the type of connection (a subclass of AbstractConnection).
template<typename TConn>
class TypedEntityConnectionMap
: public NonCopyable
, public JsonWritable {
  static_assert(std::is_base_of<AbstractConnection, TConn>::value,
                "TConn must be a subclass of AbstractConnection");
public:
  using ConnPtr = std::shared_ptr<TConn>;
  using EntityPtr = typename TConn::EntityPtr;
  using Storage = std::unordered_map<ObjectId, ConnPtr>;
  using iterator = MapToValueIterator<ObjectId, ConnPtr>;
  using const_iterator = ConstMapToValueIterator<ObjectId, ConnPtr>;

  bool addConnection(ConnPtr connection) {
    auto result = _map.insert(std::make_pair(connection->entity()->id(),
                                             connection));
    return result.second;
  }

  template<typename E, typename ...Args>
  std::pair<ConnPtr, bool>
  getOrAdd(std::shared_ptr<E> entity, Args&& ...args) {
    auto conn = getConnectionTo(entity->id());
    if (conn) {
      return std::make_pair(conn, false);
    }
    conn = std::make_shared<TConn>(entity, std::forward<Args>(args)...);
    addConnection(conn);
    return std::make_pair(conn, true);
  }

  ConnPtr getConnectionTo(ObjectId entityId) {
    auto iter = _map.find(entityId);
    if (iter == _map.end()) {
      return nullptr;
    }
    return iter->second;
  }

  EntityPtr getTarget(ObjectId entityId) {
    auto conn = getConnectionTo(entityId);
    if (!conn) {
      return nullptr;
    }
    return conn->entity();
  }

  bool containsId(ObjectId entityId) const {
    return _map.find(entityId) != _map.end();
  }

  void clear() { _map.clear(); }

  std::size_t size() const { return _map.size(); }

  bool empty() const { return _map.empty(); }

  std::size_t erase(ObjectId entityId) { return _map.erase(entityId); }

  iterator begin() { return _map.begin(); }
  iterator end() { return _map.end(); }
  const_iterator begin() const { return _map.begin(); }
  const_iterator end() const { return _map.end(); }

  // Perform an action on the entities connected to by connections
  // in the map.
  template<typename A>
  void performAction(A action) {
    for (auto& entry : _map) {
      action(entry.second->entity());
    }
  }

  // Perform an action on the connections in the map.
  template<typename A>
  void performConnectionAction(A action) {
    for (auto& entry : _map) {
      action(entry.second);
    }
  }

  // Remove connections to dead entities.
  void cullDeadConnections() {
    for (auto iter = begin();
         iter != end();) {
      if (iter->second->alive()) {
        iter++;
      } else {
        iter = _map.erase(iter);
      }
    }
  }

  Json to_json() const override {
    Json::array arr;
    for (const auto& entry : _map) {
      arr.push_back(entry.second->to_json());
    }
    return arr;
  }

  EntityPtr getRandomEntity() {
    if (empty()) {
      return nullptr;
    }
    auto index = static_cast<std::size_t>(ofRandom(0, size() - 1));
    auto conn = getConnectionAtIndex(index);
    return conn ? conn->entity() : nullptr;
  }

private:
  ConnPtr getConnectionAtIndex(std::size_t index) {
    if (index >= size()) {
      return nullptr;
    }
    auto iter = std::next(_map.begin(), index);
    return iter == _map.end() ? nullptr : iter->second;
  }

  Storage _map;
};

template<typename E>
using EntityConnectionMap = TypedEntityConnectionMap<EntityConnection<E>>;
