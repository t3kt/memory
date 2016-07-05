//
//  WorldObject.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/24/16.
//
//

#include "WorldObject.h"
#include "State.h"

static ObjectId nextId() {
  static ObjectId lastId = 0;
  return ++lastId;
}

WorldObject::WorldObject(const State& state)
: id(nextId())
, _alive(true)
, _startTime(state.time) { }

void WorldObject::output(std::ostream &os) const {
  os << typeName() << "{";
  outputFields(os);
  os << "}";
}

std::string WorldObject::typeName() const {
  return "WorldObject";
}

void WorldObject::outputFields(std::ostream& os) const {
  os << "id: " << id
  << ", alive: " << _alive
  << ", position: " << _position
  << ", startTime: " << _startTime;
}
