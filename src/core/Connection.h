//
//  Connection.h
//  memory
//
//  Created by tekt on 10/3/16.
//
//

#ifndef Connection_h
#define Connection_h

#include <unordered_map>
#include <memory>
#include "../core/Common.h"
#include "../core/JsonIO.h"
#include "../core/WorldObject.h"

class AbstractConnection
: public NonCopyable
, public JsonWritable {
public:
  virtual std::shared_ptr<WorldObject> entityBase() = 0;
  virtual bool alive() const = 0;
};

template<typename E>
class EntityConnection
: public AbstractConnection {
public:
  using EntityT = E;
  using EntityPtr = std::shared_ptr<E>;

  explicit EntityConnection(EntityPtr& entity)
  : _entity(entity) { }

  std::shared_ptr<WorldObject> entityBase() override {
    return _entity;
  }

  bool alive() const override {
    return _entity && _entity->alive();
  }

  EntityPtr& entity() { return _entity; }
  const EntityPtr& entity() const { return _entity; }

  Json to_json() const override {
    return Json::object {
      {"id", _entity->id()},
    };
  }

private:
  EntityPtr _entity;
};

template<typename TConn>
class EntityConnectionMap
: public NonCopyable
, public JsonWritable {
public:
  using ConnT = TConn;
  using ConnPtr = std::shared_ptr<ConnT>;
  using EntityPtr = typename ConnT::EntityPtr;
  using Storage = std::unordered_map<ObjectId, ConnPtr>;
  using iterator = typename Storage::iterator;
  using const_iterator = typename Storage::const_iterator;

  bool addConnection(ConnPtr connection) {
    auto result = _map.insert(std::make_pair(connection->entity()->id(),
                                             connection));
    return result.second;
  }

  template<typename E, typename ...Args>
  ConnT getOrAdd(std::shared_ptr<E> entity, Args&& ...args) {
    auto conn = getConnectionTo(entity->id());
    if (conn) {
      return conn;
    }
    conn = std::make_shared<ConnT>(entity, std::forward<Args>(args)...);
    addConnection(conn);
    return conn;
  }

  ConnPtr getConnectionTo(ObjectId entityId) {
    auto iter = _map.find(entityId);
    if (iter == end()) {
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
    auto iter = _map.find(entityId);
    return iter != end();
  }

  void clear() { _map.clear(); }

  std::size_t size() const { return _map.size(); }

  std::size_t erase(ObjectId entityId) { return _map.erase(entityId); }

  iterator begin() { return _map.begin(); }
  iterator end() { return _map.end(); }
  const_iterator begin() const { return _map.begin(); }
  const_iterator end() const { return _map.end(); }

  template<typename A>
  void performAction(A action) {
    for (auto& entry : _map) {
      action(entry->second->entity());
    }
  }

  template<typename A>
  void performConnectionAction(A action) {
    for (auto& entry : _map) {
      action(entry->second);
    }
  }

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
      arr.push_back(entry->second->to_json());
    }
    return arr;
  }

private:
  Storage _map;
};

#endif /* Connection_h */
