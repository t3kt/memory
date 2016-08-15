//
//  Serialization.h
//  memory
//
//  Created by tekt on 7/27/16.
//
//

#ifndef Serialization_h
#define Serialization_h

#include <stdexcept>
#include "../core/JsonIO.h"

class Context;

using SerializationContext = Context;

class SerializationException : std::runtime_error {
public:
  SerializationException(const std::string& message)
  : std::runtime_error(message) { }
};

class Serializable {
public:
  Json serializeFields(const SerializationContext& context) const;
  Json serializeRefs(const SerializationContext& context) const;
  virtual void deserializeFields(const Json& obj,
                                 const SerializationContext& context) = 0;
  virtual void deserializeRefs(const Json& obj,
                               SerializationContext& context) { }
protected:
  virtual void addSerializedFields(Json::object& obj,
                                   const SerializationContext& context) const = 0;
  virtual void addSerializedRefs(Json::object& obj,
                                 const SerializationContext& context) const { }
};

#endif /* Serialization_h */
