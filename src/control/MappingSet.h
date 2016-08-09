//
//  MappingSet.h
//  memory
//
//  Created by tekt on 7/29/16.
//
//

#ifndef MappingSet_h
#define MappingSet_h

#include <string>
#include "Common.h"
#include "JsonIO.h"

template<typename T>
class MappingSet
: public JsonReadable
, public JsonWritable
, public Outputable {
public:
  using Storage = std::vector<T>;
  using iterator = typename Storage::iterator;
  using const_iterator = typename Storage::const_iterator;

  Json to_json() const override {
    Json::array arr;
    for (const auto& mapping : _mappings) {
      arr.push_back(mapping.to_json());
    }
    return arr;
  }

  void read_json(const Json& arr) override {
    JsonUtil::assertHasType(arr, Json::ARRAY);
    _mappings.clear();
    for (const auto& val : arr.array_items()) {
      T mapping;
      mapping.read_json(val);
      _mappings.push_back(mapping);
    }
  }

  void add(T mapping) { _mappings.push_back(mapping); }

  iterator begin() { return _mappings.begin(); }
  iterator end() { return _mappings.end(); }
  const_iterator begin() const { return _mappings.begin(); }
  const_iterator end() const { return _mappings.end(); }

  std::size_t size() const { return _mappings.size(); }
  bool empty() const { return _mappings.empty(); }

  std::string typeName() const override { return "MappingSet"; }
protected:
  void outputFields(std::ostream& os) const override {
    for (const auto& mapping : _mappings) {
      os << mapping << ", ";
    }
  }
private:
  Storage _mappings;
};

#endif /* MappingSet_h */
