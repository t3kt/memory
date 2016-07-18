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
  
  const State& state;

protected:
  std::string typeName() const override { return "StateEventArgs"; }
  void outputFields(std::ostream& os) const override {
    os << "state: " << state;
  }
};

template<typename T>
class EntityEventArgs : public StateEventArgs {
public:
  EntityEventArgs(const State& s, T& entity) : StateEventArgs(s), _entity(entity) {}
  
  T& entity() { return _entity; }

protected:
  std::string typeName() const override { return "EntityEventArgs"; }
  void outputFields(std::ostream& os) const override {
    os << "entity: " << _entity;
    os << "state: " << state;
  }
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

  T& value() { return _value; }

protected:
  std::string typeName() const override { return "ValueEventArgs"; }
  void outputFields(std::ostream& os) const override {
    os << "value: " << _value;
  }
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
