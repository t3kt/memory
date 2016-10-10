//
//  WorldObject.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/24/16.
//
//

#include "../core/Info.h"
#include "../core/WorldObject.h"

WorldObject::WorldObject()
: _id(ObjectIds::next())
, _alive(true)
, _color(1, 1, 1, 1) { }

void WorldObject::kill() {
  _alive = false;
  this->detachConnections();
}

std::string WorldObject::typeName() const {
  return "WorldObject";
}

void WorldObject::outputFields(std::ostream& os) const {
  os << "id: " << _id
     << ", alive: " << _alive;
}

void WorldObject::fillInfo(Info &info) const {
  info.add("type:", typeName());
  info.add("id:", id());
  info.add("screen x:", screenPos().x);
  info.add("screen y:", screenPos().y);
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
