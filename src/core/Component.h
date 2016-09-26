//
//  Component.h
//  memory
//
//  Created by tekt on 9/23/16.
//
//

#ifndef Component_h
#define Component_h

#include <memory>
#include <type_traits>
#include <vector>

class ComponentBase {
public:
  virtual void setup() {}
  virtual void update() {}
  virtual void draw() {}
};

template<typename C>
class ComponentCollection {
public:
  using Storage = std::vector<std::shared_ptr<C>>;
  using iterator = typename Storage::iterator;
  using const_iterator = typename Storage::const_iterator;

  template<typename T>
  std::shared_ptr<T> add(std::shared_ptr<T> component) {
    _components.push_back(component);
    return component;
  }

  template<typename T, typename ...Args>
  std::shared_ptr<T> add(Args&& ...args) {
    auto component = std::make_shared<T>(std::forward<Args>(args)...);
    _components.push_back(component);
    component->setup();
    return component;
  }

  void update() {
    for (auto& component : _components) {
      component->update();
    }
  }

  void draw() {
    for (auto& component : _components) {
      component->draw();
    }
  }

  iterator begin() { return _components.begin(); }
  const_iterator begin() const { return _components.begin(); }
  iterator end() { return _components.end(); }
  const_iterator end() const { return _components.end(); }

private:
  Storage _components;
};

#endif /* Component_h */
