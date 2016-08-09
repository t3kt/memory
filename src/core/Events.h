//
//  Events.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef Events_h
#define Events_h

#include <ofxLiquidEvent.h>
#include "Common.h"

class EventArgs
: public Outputable {
public:
  EventArgs() : _handled(false) {}

  bool handled() const { return _handled; }
  void markHandled() { _handled = true; }
private:
  bool _handled;
};

template<typename T>
class ValueEventArgs
: public EventArgs {
public:
  ValueEventArgs(T& value)
  : _value(value) { }

  T& value() { return _value; }

  std::string typeName() const override { return "ValueEventArgs"; }
private:
  T& _value;
};

class AbstractEvent {
public:
  using VoidFunctor = std::function<void()>;

  virtual void addVoidListener(VoidFunctor functor, void* owner) = 0;
  virtual void removeListeners(void* owner) = 0;
};

template<typename ArgType>
class TEvent
: public AbstractEvent
, public ofxLiquidEvent<ArgType> {
public:
  using Functor = typename ofxLiquidEvent<ArgType>::Functor;
  using VoidFunctor = std::function<void()>;

  bool notifyListenersUntilHandled(ArgType& args) {
    for (auto listener : this->listeners) {
      listener.second(args);
      if (args.handled()) {
        return true;
      }
    }
    return false;
  }

  void operator+=(Functor functor) {
    this->addListener(functor, 0);
  }

  void operator+=(VoidFunctor functor) {
    addVoidListener(functor, 0);
  }

  void addVoidListener(VoidFunctor functor, void* owner) override {
    this->addListener([functor](ArgType&) {
      functor();
    }, owner);
  }

  void removeListeners(void* owner) override {
    ofxLiquidEvent<ArgType>::removeListeners(owner);
  }

  bool operator()(ArgType& args) {
    return notifyListenersUntilHandled(args);
  }
};

template<typename T>
using ValueEvent = TEvent<ValueEventArgs<T>>;

#endif /* Events_h */
