//
//  Serialization.cpp
//  memory
//
//  Created by tekt on 7/27/16.
//
//

#include "../core/Context.h"
#include "../core/Serialization.h"

ofJson Serializable::serializeFields(const SerializationContext &context) const {
  auto obj = ofJson::object();
  addSerializedFields(obj, context);
  return obj;
}

ofJson Serializable::serializeRefs(const SerializationContext &context) const {
  auto obj = ofJson::object();
  addSerializedRefs(obj, context);
  if (obj.empty()) {
    return nullptr;
  }
  return obj;
}
