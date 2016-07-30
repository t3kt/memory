//
//  Serialization.cpp
//  memory
//
//  Created by tekt on 7/27/16.
//
//

#include "Context.h"
#include "Serialization.h"

Json Serializable::serializeFields(const SerializationContext &context) const {
  Json::object obj;
  addSerializedFields(obj, context);
  return obj;
}

Json Serializable::serializeRefs(const SerializationContext &context) const {
  Json::object obj;
  addSerializedRefs(obj, context);
  if (obj.empty()) {
    return nullptr;
  }
  return obj;
}
