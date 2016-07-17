//
//  Events.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef Events_h
#define Events_h

#include "Common.h"
#include "State.h"

#include <vector>

#include <ofxLiquidEvent.h>

class EventArgs {
public:
  EventArgs() : _handled(false) {}

  bool handled() const { return _handled; }
  void markHandled() { _handled = true; }
private:
  bool _handled;
};

class StateEventArgs
: public Outputable
, public EventArgs {
public:
  StateEventArgs(const State& s) : state(s) {}
  
  void output(std::ostream& os) const override {
    os << "StateEventArgs{"
      << state
      << "}";
  }
  
  const State& state;
};

template<typename T>
class EntityEventArgs : public StateEventArgs {
public:
  EntityEventArgs(const State& s, T& entity) : StateEventArgs(s), _entity(entity) {}
  
  void output(std::ostream& os) const override {
    os << "EntityEventArgs{"
        << "entity: " << _entity
        << "state: " << state
        << "}";
  }
  
  T& entity() { return _entity; }
private:
  T& _entity;
};

template<typename T>
class ValueEventArgs
: public EventArgs
, public Outputable {
public:
  ValueEventArgs(T& value)
  : _value(value) { }

  void output(std::ostream& os) const override {
    os << "ValueEventArgs{"
    << "value: " << _value
    << "}";
  }

  T& value() { return _value; }
private:
  T& _value;
};

template<typename ArgType>
class TEvent
: public ofxLiquidEvent<ArgType> {
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
    this->addListener([functor](ArgType&) {
      functor();
    }, 0);
  }

  bool operator()(ArgType& args) {
    return notifyListenersUntilHandled(args);
  }
};

template<typename T>
using ValueEvent = TEvent<ValueEventArgs<T>>;

class AnimationObject;
using AnimationEventArgs = EntityEventArgs<AnimationObject>;
using AnimationEvent = TEvent<AnimationEventArgs>;

class OccurrenceEntity;
using OccurrenceEventArgs = EntityEventArgs<OccurrenceEntity>;
using OccurrenceEvent = TEvent<OccurrenceEventArgs>;

class ObserverEntity;
using ObserverEventArgs = EntityEventArgs<ObserverEntity>;
using ObserverEvent = TEvent<ObserverEventArgs>;

#endif /* Events_h */
