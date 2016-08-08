//
//  WorldObject.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/24/16.
//
//

#include "State.h"
#include "WorldObject.h"

static ObjectId nextId() {
  static ObjectId lastId = 0;
  return ++lastId;
}

WorldObject::WorldObject()
: _id(nextId())
, _alive(true)
, _alpha(1) { }

std::string WorldObject::typeName() const {
  return "WorldObject";
}

void WorldObject::outputFields(std::ostream& os) const {
  os << "id: " << _id
     << ", alive: " << _alive;
}

void WorldObject::addSerializedFields(Json::object &obj,
                                      const SerializationContext& context) const {
  JsonUtil::mergeInto(obj, {
    {"id", JsonUtil::toJson(_id)},
    {"alive", _alive},
  });
}

void WorldObject::deserializeFields(const Json &obj,
                                    const SerializationContext &context) {
  _id = JsonUtil::fromJson<ObjectId>(obj["id"]);
  _alive = JsonUtil::fromJsonField(obj, "alive", true);
}
