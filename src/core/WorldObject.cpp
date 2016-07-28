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
: _id(nextId())
, _alive(true) { }

std::string WorldObject::typeName() const {
  return "WorldObject";
}

void WorldObject::outputFields(std::ostream& os) const {
  os << "id: " << _id
     << ", alive: " << _alive;
}
