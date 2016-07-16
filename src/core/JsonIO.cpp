//
//  JsonIO.cpp
//  memory
//
//  Created by tekt on 6/30/16.
//
//

#include "JsonIO.h"

#include "AppParameters.h"

#include <ofTypes.h>
#include <ofUtils.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <initializer_list>
#include <functional>

using json11::JsonParse;

class JsonWriter {
public:
  JsonWriter(std::ostream& out)
  : _out(out)
  , _tab("  ")
  , _indent(0) { }

  void write(const Json& value) {
    switch (value.type()) {
      case Json::ARRAY:
        writeArray(value);
        break;
      case Json::OBJECT:
        writeObject(value);
        break;
      default:
        writeSimpleValue(value);
        break;
    }
  }

private:
  void writeSimpleValue(const Json& val) {
    _out << val.dump();
  }

  void writeObject(const Json& obj) {
    const auto& items = obj.object_items();
    if (items.empty()) {
      _out << "{}";
      return;
    }
    _out << "{\n";
    _indent++;
    int i = 0;
    for (const auto& item : items) {
      writeIndent();
      Json key = item.first;
      _out << key.dump() << ": ";
      write(item.second);
      if (i < (items.size() - 1)) {
        _out << ",";
      }
      _out << "\n";
      i++;
    }
    //..
    _indent--;
    writeIndent();
    _out << "}";
  }

  void writeArray(const Json& arr) {
    const auto& items = arr.array_items();
    if (items.empty()) {
      _out << "[]";
      return;
    }
    _out << "[\n";
    _indent++;
    int i = 0;
    for (const auto& item : items) {
      writeIndent();
      write(item);
      if (i < (items.size() - 1)) {
        _out << ",";
      }
      _out << "\n";
      i++;
    }
    //...
    _indent--;
    writeIndent();
    _out << "]";
  }

  void writeIndent() {
    for (int i = 0; i < _indent; ++i) {
      _out << _tab;
    }
  }

  std::string _tab;
  int _indent;
  std::ostream& _out;
};

void prettyPrintJsonToStream(const Json& value, std::ostream& os) {
  JsonWriter writer(os);
  writer.write(value);
}

std::string prettyPrintJsonToString(const Json& value) {
  std::ostringstream os;
  prettyPrintJsonToStream(value, os);
  return os.str();
}

static void assertHasShape(const Json& value, Json::shape shape) {
  std::string message;
  if (!value.has_shape(shape, message)) {
    throw JsonException(message);
  }
}

static void assertHasType(const Json& value, Json::Type type) {
  if (value.type() != type) {
    throw JsonException("Incorrect json type: " + value.dump());
  }
}

static void assertHasLength(const Json& value, int length) {
  if (value.array_items().size() != length) {
    throw JsonException("Incorrect length (should be " + ofToString(length) + "): " + value.dump());
  }
}

static Json toJsonValue(const ofVec3f& value) {
  return Json::array { value.x, value.y, value.z };
}

static Json toJsonValue(const ofFloatColor& value) {
  return Json::array { value.r, value.g, value.b, value.a };
}

static Json toJsonValue(const std::string& value) {
  return value;
}

static ofVec3f ofVec3fFromJsonValue(const Json& value) {
  assertHasType(value, Json::ARRAY);
  assertHasLength(value, 3);
  const Json& x = value[0];
  const Json& y = value[1];
  const Json& z = value[2];
  assertHasType(x, Json::NUMBER);
  assertHasType(y, Json::NUMBER);
  assertHasType(z, Json::NUMBER);
  return ofVec3f(x.number_value(),
                 y.number_value(),
                 z.number_value());
}

static ofFloatColor ofFloatColorFromJsonValue(const Json& value) {
  assertHasType(value, Json::ARRAY);
  assertHasLength(value, 4);
  const Json& r = value[0];
  const Json& g = value[1];
  const Json& b = value[2];
  const Json& a = value[3];
  assertHasType(r, Json::NUMBER);
  assertHasType(g, Json::NUMBER);
  assertHasType(b, Json::NUMBER);
  assertHasType(a, Json::NUMBER);
  return ofFloatColor(r.number_value(),
                      g.number_value(),
                      b.number_value(),
                      a.number_value());
}

static Json merge(const Json obj1, const Json obj2) {
  Json::object out(obj1.object_items());
  out.insert(obj2.object_items().begin(), obj2.object_items().end());
  return out;
}

template<>
Json TParam<ofVec3f>::to_json() const {
  return toJsonValue(get());
}

template<>
Json TParam<ofFloatColor>::to_json() const {
  return toJsonValue(get());
}

template<>
Json TParam<float>::to_json() const {
  return get();
}

template<>
Json TParam<bool>::to_json() const {
  return get();
}

template<>
Json TParam<int>::to_json() const {
  return get();
}

template<>
Json TParam<std::string>::to_json() const {
  return get();
}

template<>
void TParam<float>::read_json(const Json& val) {
  assertHasType(val, Json::NUMBER);
  set(val.number_value());
}

template<>
void TParam<int>::read_json(const Json& val) {
  assertHasType(val, Json::NUMBER);
  set(val.number_value());
}

template<>
void TParam<bool>::read_json(const Json& val) {
  assertHasType(val, Json::BOOL);
  set(val.bool_value());
}

template<>
void TParam<std::string>::read_json(const Json &val) {
  assertHasType(val, Json::STRING);
  set(val.string_value());
}

template<>
void TParam<ofVec3f>::read_json(const Json& val) {
  set(ofVec3fFromJsonValue(val));
}

template<>
void TParam<ofFloatColor>::read_json(const Json& val) {
  set(ofFloatColorFromJsonValue(val));
}

void readJsonIntoParams(const Json& obj, std::initializer_list<std::reference_wrapper<TParamBase>> params) {
  for (auto iter = params.begin();
       iter != params.end();
       iter++) {
    iter->get().readJsonField(obj);
  }
}

Json paramsToObject(std::initializer_list<std::reference_wrapper<const TParamBase>> params) {
  Json::object obj;
  for (auto iter = params.begin();
       iter != params.end();
       iter++) {
    obj.insert(iter->get().toJsonField());
  }
  return obj;
}

void Params::readJsonField(const Json& obj) {
  const Json& val = obj[getKey()];
  if (!val.is_null()) {
    assertHasType(val, Json::OBJECT);
    read_json(val);
  } else if (hasDefault()) {
    resetToDefault();
  } else {
    throw JsonException("Required field missing: " + getKey());
  }
}

Json Params::to_json() const {
  Json::object obj;
  for (auto iter = _paramBases.begin();
       iter != _paramBases.end();
       iter++) {
    obj.insert((*iter)->toJsonField());
  }
  return obj;
}

void Params::read_json(const Json &val) {
  assertHasType(val, Json::OBJECT);
  for (auto iter = _paramBases.begin();
       iter != _paramBases.end();
       iter++) {
    (*iter)->readJsonField(val);
  }
}

void MemoryAppParameters::readFromFile(std::string filepath) {
  filepath = ofToDataPath(filepath);
  if (!ofFile::doesFileExist(filepath, true)) {
    ofLogWarning() << "can't find settings file: " << filepath;
    return;
  }
  std::ifstream in(filepath.c_str());
  std::stringstream buf;
  buf << in.rdbuf();
  in.close();
  std::string jsonstr = buf.str();
  std::string error;
  Json obj = Json::parse(jsonstr, error, JsonParse::STANDARD);
  if (!error.empty()) {
    throw JsonException(error);
  }
  read_json(obj);
}

void MemoryAppParameters::writeToFile(std::string filepath) const {
  Json obj = to_json();
  filepath = ofToDataPath(filepath);
  std::ofstream out(filepath.c_str());
  prettyPrintJsonToStream(obj, out);
  out.close();
}

