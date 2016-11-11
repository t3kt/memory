//
//  ObjectManager.h
//  memory-prototype-2
//
//  Created by tekt on 6/23/16.
//
//

#pragma once

#include <algorithm>
#include <functional>
#include <list>
#include <memory>
#include <ofMath.h>
#include <ofUtils.h>
#include <ofxTCommon.h>
#include <ofxTJsonIO.h>
#include "../core/Connection.h"
#include "../core/EntityMap.h"
#include "../core/Serialization.h"

// Container that stores and owns a set of entities.
template <typename T>
class ObjectManager
: public NonCopyable {
public:
  using StorageList = std::list<std::shared_ptr<T>>;
  using iterator = typename StorageList::iterator;
  using const_iterator = typename StorageList::const_iterator;
  using EntityPtr = std::shared_ptr<T>;
  
  void add(std::shared_ptr<T> object) {
    this->_objects.push_back(object);
  }

  ofJson serializeEntities(const SerializationContext& context) const {
    auto arr = ofJson::array();
    for (const auto& entity : *this) {
      auto fields = entity->serializeFields(context);
      auto refs = entity->serializeRefs(context);
      if (refs.is_null()) {
        arr.push_back(fields);
      } else {
        arr.push_back(ofxTCommon::JsonUtil::merge(fields, refs));
      }
    }
    return arr;
  }

  void deserializeEntityFields(const ofJson& arr,
                               const SerializationContext& context) {
    ofxTCommon::JsonUtil::assertIsArray(arr);
    for (const auto& val : arr) {
      ofxTCommon::JsonUtil::assertIsObject(val);
      auto entity = T::createEmpty(context);
      entity->deserializeFields(val, context);
      add(entity);
    }
  }

  void deserializeEntityRefs(const ofJson& arr,
                             SerializationContext& context) {
    ofxTCommon::JsonUtil::assertIsArray(arr);
    for (const auto& val : arr) {
      ofxTCommon::JsonUtil::assertIsObject(val);
      ObjectId id = ofxTCommon::JsonUtil::fromJsonField(val, "id", ObjectIds::NONE);
      if (id == ObjectIds::NONE) {
        throw SerializationException("Missing ID for object: " + val.dump());
      }
      auto entity = (*this)[id];
      if (!entity) {
        throw SerializationException("Entity not found: " + ofToString(id));
      }
      entity->deserializeRefs(val, context);
    }
  }

  void loadDeserializedRefsInto(EntityMap<T>& entities,
                                const ofJson& idArray) {
    if (idArray.is_null()) {
      return;
    }
    ofxTCommon::JsonUtil::assertIsArray(idArray);
    for (const auto& val : idArray) {
      auto id = ofxTCommon::JsonUtil::fromJson<ObjectId>(val);
      auto entity = (*this)[id];
      if (!entity) {
        throw SerializationException("Entity not found: " + ofToString(id));
      }
      entities.add(entity);
    }
  }

  template<typename TConn>
  void loadDeserializedRefsInto(ParticlePtr source,
                                TypedEntityConnectionMap<TConn>& connections,
                                const ofJson& array) {
    if (array.is_null()) {
      return;
    }
    ofxTCommon::JsonUtil::assertIsArray(array);
    for (const auto& val : array) {
      ofxTCommon::JsonUtil::assertIsObject(val);
      auto id = ofxTCommon::JsonUtil::fromJson<ObjectId>(val["id"]);
      auto entity = (*this)[id];
      if (!entity) {
        throw SerializationException("Entity not found: " + ofToString(id));
      }
      connections.getOrAdd(source, entity);
    }
  }

  // Remove dead entities from the manager.
  void cullObjects() {
    processAndCullObjects([](EntityPtr&) { });
  }

  // Perform an action on each entity. After performing the action,
  // if the entity has died, remove it from the manager.
  void processAndCullObjects(std::function<void(EntityPtr&)> callback) {
    for (auto i = begin();
         i != end();) {
      auto& object = *i;
      callback(object);
      if (object->alive()) {
        i++;
      } else {
        i = _objects.erase(i);
      }
    }
  }

  // Perform an action on each entity owned by the manager
  template<typename A>
  void performAction(A action) {
    for (EntityPtr& entity : _objects) {
      action(entity);
    }
  }

  void clear() { _objects.clear(); }
  std::size_t size() const { return _objects.size(); }
  bool empty() const { return _objects.empty(); }
  iterator begin() { return _objects.begin(); }
  iterator end() { return _objects.end(); }
  const_iterator begin() const { return _objects.begin(); }
  const_iterator end() const { return _objects.end(); }

  EntityPtr getAtIndex(std::size_t index) {
    if (index >= size()) {
      return EntityPtr();
    }
    auto iter = std::next(begin(), index);
    if (iter != end()) {
      return *iter;
    }
    return EntityPtr();
  }

  EntityPtr operator[](ObjectId id) {
    for (auto& entity : _objects) {
      if (entity->id() == id) {
        return entity;
      }
    }
    return EntityPtr();
  }

  // Gets the entity before nextEntity, if any, or null otherwise
  EntityPtr before(const EntityPtr& nextEntity) {
    if (!nextEntity) {
      return nullptr;
    }
    auto iter = std::find(begin(), end(), nextEntity);
    if (iter == begin()) {
      return nullptr;
    }
    --iter;
    if (iter == begin()) {
      return nullptr;
    }
    return *iter;
  }

  // Gets the entity before nextEntity, if any, or the last entity
  // in the manager otherwise. Used for looping through the entities.
  EntityPtr beforeOrLast(const EntityPtr& nextEntity) {
    if (empty()) {
      return nullptr;
    }
    auto entity = before(nextEntity);
    if (entity) {
      return entity;
    }
    return last();
  }

  // Gets the entity after prevEntity, if any, or null otherwise
  EntityPtr after(const EntityPtr& prevEntity) {
    if (!prevEntity) {
      return nullptr;
    }
    auto iter = std::find(begin(), end(), prevEntity);
    if (iter == end()) {
      return nullptr;
    }
    ++iter;
    if (iter == end()) {
      return nullptr;
    }
    return *iter;
  }

  // Gets the entity after prevEntity, if any, or the first entity
  // in the manager otherwise. Used for looping through the entities.
  EntityPtr afterOrFirst(const EntityPtr& prevEntity) {
    if (empty()) {
      return nullptr;
    }
    auto entity = after(prevEntity);
    if (entity) {
      return entity;
    }
    return first();
  }

  EntityPtr first() { return empty() ? nullptr : _objects.front(); }
  EntityPtr last() { return empty() ? nullptr : _objects.back(); }

  EntityPtr getRandomEntity() {
    if (empty()) {
      return nullptr;
    }
    auto index = static_cast<int>(ofRandom(0, size() - 1));
    return getAtIndex(index);
  }

private:
  StorageList _objects;
};
