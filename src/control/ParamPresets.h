//
//  ParamPresets.h
//  memory
//
//  Created by tekt on 10/11/16.
//
//

#ifndef ParamPresets_h
#define ParamPresets_h

#include <utility>
#include "../control/Params.h"
#include "../core/JsonIO.h"

class ParamPreset
: public JsonReadable
, public JsonWritable {
public:
  using const_iterator = Json::object::const_iterator;

  ParamPreset()
  : _values(Json::object()) {}

  const std::string& name() const { return _name; }
  void setName(const std::string& name) { _name = name; }

  void read_json(const Json& obj) override;
  Json to_json() const override;

  void captureParams(const Params& params);
  void applyToParams(Params& params) const;

  bool containsKey(const std::string& key) const {
    const auto& obj = _values.object_items();
    const auto iter = obj.find(key);
    return iter != obj.end() && !iter->second.is_null();
  }
  const Json& getJsonValue(const std::string& key) const {
    return _values[key];
  }

  template<typename T>
  T getValue(const std::string& key) const {
    const Json& jsonVal = getJsonValue(key);
    return JsonUtil::fromJson<T>(jsonVal);
  }

  const_iterator begin() const {
    return _values.object_items().begin();
  }
  const_iterator end() const {
    return _values.object_items().end();
  }

  const Json& values() const { return _values; }

  void stripUnsupportedParams(const Params& params);

private:
  std::string _name;
  Json _values;
};

#endif /* ParamPresets_h */
