//
//  JsonIO.h
//

#pragma once

#include <algorithm>
#include <iostream>
#include <json11.hpp>
#include <ofxTEnums.h>

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

  template<typename T, typename Iter>
  Json toJsonArray(Iter begin, Iter end) {
    Json::array arr;
    for (auto iter = begin; iter != end; iter++) {
      arr.push_back(toJson<T>(*iter));
    }
    return arr;
  }

  template<typename T, typename Iter>
  Json toJsonArrayOrNull(Iter begin, Iter end) {
    if (begin == end) {
      return nullptr;
    }
    return toJsonArray<T, Iter>(begin, end);
  }

  template<typename T>
  std::vector<T> fromJsonArray(const Json& value) {
    assertHasType(value, Json::ARRAY);
    std::vector<T> result;
    for (const auto& val : value.array_items()) {
      result.push_back(fromJson<T>(val));
    }
    return result;
  }

  template<typename T>
  std::vector<T> fromJsonArrayOrNull(const Json& value) {
    if (value.is_null()) {
      return std::vector<T>();
    }
    return fromJsonArray<T>(value);
  }

  template<typename T>
  typename std::enable_if<std::is_enum<T>::value, T>::type
  enumFromJson(const Json& value) {
    assertHasType(value, Json::STRING);
    return ofxTCommon::parseEnum<T>(value.string_value());
  }

  template<typename T>
  Json enumToJson(const T& value) {
    return ofxTCommon::enumToString(value);
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
  void prettyPrintJsonToFile(const Json& value, std::string filepath);
}

