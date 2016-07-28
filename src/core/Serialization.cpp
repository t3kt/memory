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
  addSerializedFields(obj);
  return obj;
}

Json Serializable::serializeRefs(const SerializationContext &context) const {
  Json::object obj;
  addSerializedRefs(obj);
  if (obj.empty()) {
    return nullptr;
  }
  return obj;
}

SerializationContext::SerializationContext(Context& context)
: _context(context)
, _baseTime(context.time()) { }
