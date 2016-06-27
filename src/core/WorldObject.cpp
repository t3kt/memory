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

WorldObject::WorldObject()
: id(nextId()) { }

void WorldObject::output(std::ostream &os) const {
  os << typeName() << "{";
  outputFields(os);
  os << "}";
}

std::string WorldObject::typeName() const {
  return "WorldObject";
}

void WorldObject::outputFields(std::ostream& os) const {
  os << "id: " << id;
}

StandardWorldObject::StandardWorldObject()
: WorldObject()
, _alive(true) { }

void StandardWorldObject::outputFields(std::ostream& os) const {
  WorldObject::outputFields(os);
  os << ", alive: " << _alive;
}
