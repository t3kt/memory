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
: id(nextId())
, _alive(true) { }


void WorldObject::output(std::ostream &os) const {
  os << "WorldObject{id:" << id << "}";
}
