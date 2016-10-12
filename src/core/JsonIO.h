//
//  JsonIO.h
//  memory
//
//  Created by tekt on 6/30/16.
//
//

#ifndef JsonIO_h
#define JsonIO_h

#include <iostream>
#include <json11.hpp>
#include "../core/Enums.h"

using json11::Json;

class JsonException {
public:
  JsonException(std::string msg) : message(msg) { }
  std::string message;
};

class JsonReadable {
public:
  virtual void read_json(const Json& val) = 0;
  void readFromFile(std::string filepath);
};

class JsonWritable {
public:
  virtual Json to_json() const = 0; // json11 library requires this naming

  std::string toJsonString() const;
  void writeJsonTo(std::ostream& os) const;
  void writeToFile(std::string filepath) const;
};

namespace JsonUtil {
  void assertHasShape(const Json& value, Json::shape shape);
  void assertHasType(const Json& value, Json::Type type);
  void assertHasLength(const Json& value, int length);

  template<typename T>
  Json toJson(const T& value);

  template<typename T>
  T fromJson(const Json& value);

  template<typename T>
  typename std::enable_if<std::is_enum<T>::value, T>::type
  enumFromJson(const Json& value) {
    assertHasType(value, Json::STRING);
    return parseEnum<T>(value.string_value());
  }

  template<typename T>
  Json enumToJson(const T& value) {
    return enumToString(value);
  }

  template<typename T>
  T fromJsonField(const Json& obj,
                  const std::string& key,
                  const T& defaultValue) {
    Json val = obj[key];
    if (val.is_null()) {
      return defaultValue;
    } else {
      return fromJson<T>(val);
    }
  }

  template<typename T>
  T fromJsonField(const Json& obj,
                  const std::string& key) {
    T defaultValue;
    return fromJsonField(obj, key, defaultValue);
  }

  Json merge(const Json obj1, const Json obj2);
  void mergeInto(Json::object& targetObj, const Json::object& sourceObj);

  void prettyPrintJsonToStream(const Json& value, std::ostream& os);
}

#endif /* JsonIO_h */
