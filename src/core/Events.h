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

class StateEventArgs : public Outputable {
public:
  StateEventArgs(const State& s) : state(s) {}
  
  void output(std::ostream& os) const override;
  
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

class AnimationObject;
using AnimationEventArgs = EntityEventArgs<AnimationObject>;
using AnimationEvent = ofxLiquidEvent<AnimationEventArgs>;

class OccurrenceEntity;
using OccurrenceEventArgs = EntityEventArgs<OccurrenceEntity>;
using OccurrenceEvent = ofxLiquidEvent<OccurrenceEventArgs>;

class ObserverEntity;
using ObserverEventArgs = EntityEventArgs<ObserverEntity>;
using ObserverEvent = ofxLiquidEvent<ObserverEventArgs>;

#endif /* Events_h */
