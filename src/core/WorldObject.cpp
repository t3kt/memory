//
//  WorldObject.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/24/16.
//
//

#include "WorldObject.h"
#include "State.h"

namespace JsonUtil {
  template<>
  Json toJson(const ObjectId& value) {
    return static_cast<int>(value);
  }

  template<>
  ObjectId fromJson<ObjectId>(const Json& value) {
    assertHasType(value, Json::NUMBER);
    return static_cast<ObjectId>(value.int_value());
  }
}

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
