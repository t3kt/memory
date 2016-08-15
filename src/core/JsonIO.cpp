//
//  JsonIO.cpp
//  memory
//
//  Created by tekt on 6/30/16.
//
//

#include <iostream>
#include <fstream>
#include <functional>
#include <ofTypes.h>
#include <ofUtils.h>
#include <sstream>
#include "../app/AppSystem.h"
#include "../core/JsonIO.h"

using json11::JsonParse;
using namespace JsonUtil;

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
    std::size_t i = 0;
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
    std::size_t i = 0;
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

static std::string prettyPrintJsonToString(const Json& value) {
  std::ostringstream os;
  prettyPrintJsonToStream(value, os);
  return os.str();
}

namespace JsonUtil {

  void prettyPrintJsonToStream(const Json& value, std::ostream& os) {
    JsonWriter writer(os);
    writer.write(value);
  }

  void assertHasShape(const Json& value, Json::shape shape) {
    std::string message;
    if (!value.has_shape(shape, message)) {
      throw JsonException(message);
    }
  }

  void assertHasType(const Json& value, Json::Type type) {
    if (value.type() != type) {
      throw JsonException("Incorrect json type: " + value.dump());
    }
  }

  void assertHasLength(const Json& value, int length) {
    if (value.array_items().size() != length) {
      throw JsonException("Incorrect length (should be " + ofToString(length) + "): " + value.dump());
    }
  }

  template<>
  Json toJson(const float& value) {
    return value;
  }

  template<>
  Json toJson(const int& value) {
    return value;
  }

  template<>
  Json toJson(const bool& value) {
    return value;
  }

  template<>
  Json toJson(const std::string& value) {
    return value;
  }

  template<>
  Json toJson(const ofVec3f& value) {
    return Json::array { value.x, value.y, value.z };
  }

  template<>
  Json toJson(const ofFloatColor& value) {
    return Json::array { value.r, value.g, value.b, value.a };
  }

  template<>
  ofVec3f fromJson<ofVec3f>(const Json& value) {
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

  template<>
  ofFloatColor fromJson<ofFloatColor>(const Json& value) {
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

  template<>
  float fromJson<float>(const Json& value) {
    assertHasType(value, Json::NUMBER);
    return value.number_value();
  }

  template<>
  int fromJson<int>(const Json& value) {
    assertHasType(value, Json::NUMBER);
    return value.number_value();
  }

  template<>
  bool fromJson<bool>(const Json& value) {
    assertHasType(value, Json::BOOL);
    return value.bool_value();
  }

  template<>
  std::string fromJson<std::string>(const Json& value) {
    assertHasType(value, Json::STRING);
    return value.string_value();
  }

  void mergeInto(Json::object& targetObj,
                 const Json::object& sourceObj) {
    targetObj.insert(sourceObj.begin(),
                     sourceObj.end());
  }

  Json merge(const Json obj1, const Json obj2) {
    Json::object out(obj1.object_items());
    mergeInto(out, obj2.object_items());
    return out;
  }
}

static bool tryReadJsonFromFile(std::string filepath, Json* obj) {
  filepath = ofToDataPath(filepath);
  if (!ofFile::doesFileExist(filepath, true)) {
    AppSystem::get().log().app().logWarning("can't find settings file: " + filepath);
    return false;
  }
  std::ifstream in(filepath.c_str());
  std::stringstream buf;
  buf << in.rdbuf();
  in.close();
  std::string jsonstr = buf.str();
  std::string error;
  *obj = Json::parse(jsonstr, error, JsonParse::STANDARD);
  if (!error.empty()) {
    throw JsonException(error);
  }
  return true;
}

void JsonReadable::readFromFile(std::string filepath) {
  Json obj;
  if (!tryReadJsonFromFile(filepath, &obj)) {
    AppSystem::get().log().app().logWarning("can't find settings file: " + filepath);
    return;
  }
  read_json(obj);
}

void JsonWritable::writeToFile(std::string filepath) const {
  Json obj = to_json();
  filepath = ofToDataPath(filepath);
  std::ofstream out(filepath.c_str());
  prettyPrintJsonToStream(obj, out);
  out.close();
}

void JsonWritable::writeJsonTo(std::ostream& os) const {
  prettyPrintJsonToStream(to_json(), os);
}

std::string JsonWritable::toJsonString() const {
  return prettyPrintJsonToString(to_json());
}

