//
//  Serialization.h
//  memory
//
//  Created by tekt on 7/27/16.
//
//

#ifndef Serialization_h
#define Serialization_h

#include "JsonIO.h"

class Context;
class SerializationContext;

class Serializable {
public:
  Json serializeFields(const SerializationContext& context) const;
  Json serializeRefs(const SerializationContext& context) const;
  virtual void deserializeFields(const Json& obj,
                                 const SerializationContext& context) = 0;
  virtual void deserializeRefs(const Json& obj,
                               SerializationContext& context) { }
protected:
  virtual void addSerializedFields(Json::object& obj) const = 0;
  virtual void addSerializedRefs(Json::object& obj) const { }
};

class SerializationContext {
public:
  SerializationContext(Context& context);

  float baseTime() const { return _baseTime; }
private:
  float _baseTime;
  Context& _context;
};

#endif /* Serialization_h */
