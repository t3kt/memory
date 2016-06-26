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

using std::vector;
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
  EntityEventArgs(const State& s, shared_ptr<T> entity) : StateEventArgs(s), _entity(entity) {}
  
  void output(std::ostream& os) const override {
    os << "EntityEventArgs{"
        << "entity: " << *_entity
        << "state: " << state
        << "}";
  }
  
  shared_ptr<T> entity() { return _entity; }
private:
  shared_ptr<T> _entity;
};

#endif /* Events_h */
