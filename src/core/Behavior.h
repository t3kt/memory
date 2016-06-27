//
//  Behavior.h
//  memory-prototype-2
//
//  Created by tekt on 6/26/16.
//
//

#ifndef Behavior_h
#define Behavior_h

#include <functional>
#include <vector>
#include "State.h"

template <typename E>
class Behavior {
public:
  virtual void update(E& entity, const State& state) = 0;
};

template <typename E, typename P>
class BehaviorWithParams {
public:
  explicit BehaviorWithParams(const P& params) : _params(params) { }

  virtual void update(E& entity, const State& state) = 0;

protected:
  const P& _params;
};

template<typename E>
class FunctionBehavior : public Behavior<E> {
public:
  using UpdateFunction = std::function<void(E&, const State&)>;

  FunctionBehavior(UpdateFunction update) : _update(update) { }

  void update(E& entity, const State& state) override {
    _update(entity, state);
  }

private:
  const UpdateFunction _update;
};

template<typename E, typename P>
class FunctionBehaviorWithParams : public BehaviorWithParams<E, P> {
public:
  using UpdateFunction = std::function<void(E&, const State&, const P&)>;

  FunctionBehaviorWithParams(UpdateFunction update, const P& params)
  : BehaviorWithParams<E, P>(params)
  , _update(update) { }

  void update(E& entity, const State& state) override {
    _update(entity, state, BehaviorWithParams<E, P>::_params);
  }

private:
  const UpdateFunction _update;
};

template<typename E>
class BehaviorCollection : public Behavior<E> {
public:
//  template<typename ...Args>
//  void add(shared_ptr<Behavior<E>> behavior, Args&... others) {
//    add(behavior);
//    add(others...);
//  }

  void add(shared_ptr<Behavior<E>> behavior) {
    _behaviors.push_back(behavior);
  }

  void update(E& entity, const State& state) override {
    for (auto behavior : _behaviors) {
      behavior->update(entity, state);
    }
  }
private:
  std::vector<shared_ptr<Behavior<E>>> _behaviors;
};

#endif /* Behavior_h */
