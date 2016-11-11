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
#include <ofxTCommon.h>
#include <ofxTJsonIO.h>
#include <unordered_map>
#include <type_traits>
#include "../core/ParticleObject.h"
#include "../core/WorldObject.h"

// Base class for a connection to an entity (generally from
// another entity).
class AbstractConnection
: public ofxTCommon::NonCopyable
, public ofxTCommon::JsonWritable {
public:
  AbstractConnection(ParticlePtr source)
  : _sourceEntity(source) { }

  bool alive() const {
    return targetEntity().alive() && _sourceEntity->alive();
  }

  bool visible() const {
    return targetEntity().visible() && _sourceEntity->visible();
  }

  ObjectId entityId() const {
    return targetEntity().id();
  }
  ObjectId sourceId() const {
    return _sourceEntity->id();
  }

  ParticleObject& sourceEntity() { return *_sourceEntity; }
  const ParticleObject& sourceEntity() const { return *_sourceEntity; }

  virtual ParticleObject& targetEntity() = 0;
  virtual const ParticleObject& targetEntity() const = 0;

  const ofVec3f& sourcePosition() const {
    return _sourceEntity->position();
  }
  const ofVec3f& endPosition() const {
    return targetEntity().position();
  }

  ofVec3f evaluatePosition(float percentage) const {
    return sourcePosition().getInterpolated(endPosition(),
                                            percentage);
  }
private:
  ParticlePtr _sourceEntity;
};

// A connection to an entity of type E.
template<typename E>
class EntityConnection
: public AbstractConnection {
public:
  using EntityT = E;
  using EntityPtr = std::shared_ptr<E>;

  explicit EntityConnection(ParticlePtr source,
                            EntityPtr entity)
  : AbstractConnection(source)
  , _entity(entity) { }

  EntityPtr& entity() { return _entity; }
  const EntityPtr& entity() const { return _entity; }

  ofJson toJson() const override {
    return {
      {"id", entityId()},
      {"srcid", sourceId()},
    };
  }
  ParticleObject& targetEntity() override {
    return *_entity;
  }
  const ParticleObject& targetEntity() const override {
    return *_entity;
  }
private:
  EntityPtr _entity;
};

// A map of connections to various entities.
// TConn is the type of connection (a subclass of AbstractConnection).
template<typename TConn>
class TypedEntityConnectionMap
: public ofxTCommon::NonCopyable
, public ofxTCommon::JsonWritable {
  static_assert(std::is_base_of<AbstractConnection, TConn>::value,
                "TConn must be a subclass of AbstractConnection");
public:
  using ConnPtr = std::shared_ptr<TConn>;
  using EntityPtr = typename TConn::EntityPtr;
  using Storage = std::unordered_map<ObjectId, ConnPtr>;
  using iterator = MapToValueIterator<ObjectId, ConnPtr>;
  using const_iterator = ConstMapToValueIterator<ObjectId, ConnPtr>;

  bool addConnection(ConnPtr connection) {
    auto result = _map.insert(std::make_pair(connection->entityId(),
                                             connection));
    return result.second;
  }

  template<typename E, typename ...Args>
  std::pair<ConnPtr, bool>
  getOrAdd(ParticlePtr source,
           std::shared_ptr<E> entity,
           Args&& ...args) {
    auto conn = getConnectionTo(entity->id());
    if (conn) {
      return std::make_pair(conn, false);
    }
    conn = std::make_shared<TConn>(source,
                                   entity,
                                   std::forward<Args>(args)...);
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

  ofJson toJson() const override {
    auto arr = ofJson::array();
    for (const auto& entry : _map) {
      arr.push_back(entry.second->toJson());
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
