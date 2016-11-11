//
//  WorldObject.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/24/16.
//
//

#include "../core/Info.h"
#include "../core/WorldObject.h"

WorldObject::WorldObject(const ClockState& state)
: _id(ObjectIds::next())
, _alive(true)
, _color(1, 1, 1, 1)
, _age(state) { }

void WorldObject::kill() {
  _alive = false;
  this->detachConnections();
}

std::string WorldObject::typeName() const {
  return "WorldObject";
}

void WorldObject::outputFields(std::ostream& os) const {
  os << "id: " << _id
     << ", alive: " << _alive
     << ", age: " << age();
}

void WorldObject::fillInfo(Info &info) const {
  info.add("type:", typeName());
  info.add("id:", id());
  info.add("age:", age());
}

void WorldObject::addSerializedFields(ofJson &obj,
                                      const SerializationContext& context) const {
  ofxTCommon::JsonUtil::mergeInto(obj, {
    {"id", ofxTCommon::JsonUtil::toJson(_id)},
    {"alive", _alive},
    {"age", age().get()},
  });
}

void WorldObject::deserializeFields(const ofJson &obj,
                                    const SerializationContext &context) {
  _id = ofxTCommon::JsonUtil::fromJson<ObjectId>(obj["id"]);
  _alive = ofxTCommon::JsonUtil::fromJsonField(obj, "alive", true);
  _age.setAge(ofxTCommon::JsonUtil::fromJsonField(obj, "age", 0));
}
