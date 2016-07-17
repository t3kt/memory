//
//  ObjectManager.h
//  memory-prototype-2
//
//  Created by tekt on 6/23/16.
//
//

#ifndef ObjectManager_h
#define ObjectManager_h

#include <functional>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include "Common.h"
#include "Events.h"
#include "State.h"
#include "WorldObject.h"

template<typename T, typename Storage>
class AbstractObjectView {
public:
  using iterator = typename Storage::iterator;
  using const_iterator = typename Storage::const_iterator;

  AbstractObjectView(Storage objects)
  : _objects(objects) { }

  void performAction(std::function<void(std::shared_ptr<T>)> action) {
    for (auto& entity : _objects) {
      action(entity);
    }
  }

  template<typename Base>
  void performTypedAction(std::function<void(Base*)> action) {
    for (auto& entity : _objects) {
      action(entity.get());
    }
  }

  template<typename Base>
  void performTypedAction(std::function<void(const Base*)> action) const {
    for (const auto& entity : _objects) {
      action(entity.get());
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

  View& view() {
    if (!_view) {
      _view = std::make_shared<View>(*this);
    }
    return *_view;
  }

private:
  std::shared_ptr<View> _view;
};

#endif /* ObjectManager_h */
