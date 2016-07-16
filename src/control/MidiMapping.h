//
//  MidiMapping.h
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#ifndef MidiMapping_h
#define MidiMapping_h

#include <string>
#include "JsonIO.h"
#include "MidiCommon.h"

class MidiMapping
: public JsonReadable
, public JsonWritable {
public:
  MidiMapping() {}
  MidiMapping(MidiMappingKey key, std::string path)
  : _key(key)
  , _path(path) {}

  Json to_json() const override;
  void read_json(const Json& obj) override;

private:
  MidiMappingKey _key;
  std::string _path;
};

class MidiMappingSet
: public JsonReadable
, public JsonWritable {
public:
  using Storage = std::vector<MidiMapping>;
  using iterator = Storage::iterator;
  using const_iterator = Storage::const_iterator;

  Json to_json() const override;
  void read_json(const Json& obj) override;

  iterator begin() { return _mappings.begin(); }
  iterator end() { return _mappings.end(); }
  const_iterator begin() const { return _mappings.begin(); }
  const_iterator end() const { return _mappings.end(); }
private:
  Storage _mappings;
};

#endif /* MidiMapping_h */
