//
//  ObjectManager.h
//  memory-prototype-2
//
//  Created by tekt on 6/23/16.
//
//

#ifndef ObjectManager_h
#define ObjectManager_h

#include <algorithm>
#include <functional>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <ofMath.h>
#include <ofUtils.h>
#include "../core/Common.h"
#include "../core/EntityMap.h"
#include "../core/Events.h"
#include "../core/Serialization.h"
#include "../core/State.h"
#include "../core/WorldObject.h"

template<typename T, typename Storage>
class AbstractObjectView {
public:
  using iterator = typename Storage::iterator;
  using const_iterator = typename Storage::const_iterator;
  using EntityPtr = std::shared_ptr<T>;

  AbstractObjectView(Storage objects)
  : _objects(objects) { }

  void performAction(PtrRefAction<T> action) {
    for (EntityPtr& entity : _objects) {
      action(entity);
    }
  }

  template<typename Base>
  void performTypedAction(PtrAction<Base> action) {
    for (EntityPtr& entity : _objects) {
      action(entity);
    }
  }

  template<typename Base>
  void performTypedAction(PtrAction<Base> action) const {
    for (const EntityPtr& entity : _objects) {
      action(entity);
    }
  }

  std::size_t size() const {
    return _objects.size();
  }

  bool empty() const {
    return _objects.empty();
  }

  iterator begin() {
    return _objects.begin();
  }

  iterator end() {
    return _objects.end();
  }

  const_iterator begin() const {
    return _objects.cbegin();
  }

  const_iterator end() const {
    return _objects.cend();
  }

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

protected:
  Storage _objects;
};

template <typename T>
class ObjectManager
: public AbstractObjectView<T, std::list<std::shared_ptr<T>>>
, public NonCopyable {
public:
  using Manager = ObjectManager<T>;
  using StorageList = std::list<std::shared_ptr<T>>;
  using BaseView = AbstractObjectView<T, StorageList>;

  class View
  : public AbstractObjectView<T, Manager&>
  , public NonCopyable {
  public:
    using BaseView = AbstractObjectView<T, Manager&>;

    View(Manager& manager) : BaseView(manager) { }
  };

  ObjectManager()
  : BaseView(StorageList()) { }

  void cullDeadObjects(std::function<void(std::shared_ptr<T>)> callback) {
    for (auto i = this->begin();
         i != this->end();) {
      auto& object = *i;
      if (object->alive()) {
        i++;
      } else {
        callback(object);
        i = this->_objects.erase(i);
      }
    }
  }
  
  void add(std::shared_ptr<T> object) {
    this->_objects.push_back(object);
  }

  void clear() {
    this->_objects.clear();
  }

  View& view() {
    if (!_view) {
      _view = std::make_shared<View>(*this);
    }
    return *_view;
  }

  Json serializeEntities(const SerializationContext& context) const {
    Json::array arr;
    for (const auto& entity : *this) {
      Json fields = entity->serializeFields(context);
      Json refs = entity->serializeRefs(context);
      if (refs.is_null()) {
        arr.push_back(fields);
      } else {
        arr.push_back(JsonUtil::merge(fields, refs));
      }
    }
    return arr;
  }

  void deserializeEntityFields(const Json& arr,
                               const SerializationContext& context) {
    JsonUtil::assertHasType(arr, Json::ARRAY);
    for (const auto& val : arr.array_items()) {
      JsonUtil::assertHasType(val, Json::OBJECT);
      auto entity = T::createEmpty();
      entity->deserializeFields(val, context);
      add(entity);
    }
  }

  void deserializeEntityRefs(const Json& arr,
                             SerializationContext& context) {
    JsonUtil::assertHasType(arr, Json::ARRAY);
    for (const auto& val : arr.array_items()) {
      JsonUtil::assertHasType(val, Json::OBJECT);
      ObjectId id = JsonUtil::fromJsonField(val, "id", NO_OBJECT_ID);
      if (id == NO_OBJECT_ID) {
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
                                const Json& idArray) {
    if (idArray.is_null()) {
      return;
    }
    JsonUtil::assertHasType(idArray, Json::ARRAY);
    for (const auto& val : idArray.array_items()) {
      auto id = JsonUtil::fromJson<ObjectId>(val);
      auto entity = (*this)[id];
      if (!entity) {
        throw SerializationException("Entity not found: " + ofToString(id));
      }
      entities.add(entity);
    }
  }

private:
  std::shared_ptr<View> _view;
};

template<typename E>
std::shared_ptr<E> getRandomEntity(ObjectManager<E>& entities) {
  if (entities.empty()) {
    return std::shared_ptr<E>();
  }
  auto index = static_cast<int>(ofRandom(0, entities.size() - 1));
  return entities.getAtIndex(index);
}

#endif /* ObjectManager_h */
