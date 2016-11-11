//
//  Serialization.h
//  memory
//
//  Created by tekt on 7/27/16.
//
//

#ifndef Serialization_h
#define Serialization_h

#include <ofxTJsonIO.h>
#include <stdexcept>

class Context;

using SerializationContext = Context;

class SerializationException : std::runtime_error {
public:
  SerializationException(const std::string& message)
  : std::runtime_error(message) { }
};

class Serializable {
public:
  ofJson serializeFields(const SerializationContext& context) const;
  ofJson serializeRefs(const SerializationContext& context) const;
  virtual void deserializeFields(const ofJson& obj,
                                 const SerializationContext& context) = 0;
  virtual void deserializeRefs(const ofJson& obj,
                               SerializationContext& context) { }
protected:
  virtual void addSerializedFields(ofJson& obj,
                                   const SerializationContext& context) const = 0;
  virtual void addSerializedRefs(ofJson& obj,
                                 const SerializationContext& context) const { }
};

#endif /* Serialization_h */
