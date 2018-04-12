//
//  MappingSet.h
//  memory
//
//  Created by tekt on 7/29/16.
//
//

#ifndef MappingSet_h
#define MappingSet_h

#include <ofxTCommon.h>
#include <ofxTJsonIO.h>
#include <string>

// A serializable set of control mappings (like keyboard or MIDI).
// Type T is expected to implement JsonReadable and JsonWritable.
template<typename T>
class MappingSet
: public ofxTCommon::JsonReadable
, public ofxTCommon::JsonWritable
, public ofxTCommon::Outputable
, public ofxTCommon::NonCopyable {
public:
  using Storage = std::vector<T>;
  using iterator = typename Storage::iterator;
  using const_iterator = typename Storage::const_iterator;

  ofJson toJson() const override {
    auto arr = ofJson::array();
    for (const auto& mapping : *this) {
      arr.push_back(mapping.toJson());
    }
    return arr;
  }

  void readJson(const ofJson& arr) override {
    ofxTCommon::JsonUtil::assertIsArray(arr);
    _mappings.clear();
    for (const auto& val : arr) {
      T mapping;
      mapping.readJson(val);
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
