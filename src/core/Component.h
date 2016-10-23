//
//  Component.h
//

#pragma once

#include <memory>
#include <type_traits>
#include <vector>

// Interface for a component that has a setup method.
class SetupableComponent {
public:
  virtual void setup() {}
};

// Interface for a component that has an update method.
class UpdatableComponent {
public:
  virtual void update() {}
};

// Interface for a component that has a draw method.
class DrawableComponent {
public:
  virtual void draw() {}
};

// Combination of setup/update/draw method interfaces
class ComponentBase
: public SetupableComponent
, public UpdatableComponent
, public DrawableComponent {
};

class MultiComponentCollection {
public:
  using SetupT = std::shared_ptr<SetupableComponent>;
  using UpdateT = std::shared_ptr<UpdatableComponent>;
  using DrawT = std::shared_ptr<DrawableComponent>;
  template<typename T>
  using Storage = std::vector<T>;

  template<typename T, typename ...Args>
  std::shared_ptr<T> add(Args&& ...args) {
    auto component = std::make_shared<T>(std::forward<Args>(args)...);
    callSetup(component);
    registerUpdate(component);
    registerDraw(component);
    return component;
  }

  void update();
  void draw();

private:
  template<typename T>
  void callSetup(std::shared_ptr<T> component) {}
  template<
  typename T,
  typename = typename std::enable_if<
  std::is_base_of<SetupableComponent, T>::value>::type>
  void callSetup(std::shared_ptr<T> component) {
    component->setup();
  }

  template<typename T>
  void registerUpdate(std::shared_ptr<T> component) {}

  template<
  typename T,
  typename = typename std::enable_if<
    std::is_base_of<UpdatableComponent, T>::value>::type>
  void registerUpdate(std::shared_ptr<T> component) {
    _updates.push_back(component);
  }

  template<typename T>
  void registerDraw(std::shared_ptr<T> component) {}

  template<
  typename T,
  typename = typename std::enable_if<
  std::is_base_of<DrawableComponent, T>::value>::type>
  void registerDraw(std::shared_ptr<T> component) {
    _draws.push_back(component);
  }

  Storage<UpdateT> _updates;
  Storage<DrawT> _draws;
};

// A collection of components of type C, with methods for adding new
// components, and calling update/draw on each component.
template<typename C>
class ComponentCollection {
public:
  using Storage = std::vector<std::shared_ptr<C>>;
  using iterator = typename Storage::iterator;
  using const_iterator = typename Storage::const_iterator;

  // Add an externally constructed component to the collection. Does
  // not call setup() since it is assumed to have already been called.
  template<typename T>
  std::shared_ptr<T> add(std::shared_ptr<T> component) {
    _components.push_back(component);
    return component;
  }

  // Construct a new component of type T, call its setup() method, and
  // add it to the collection.
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

