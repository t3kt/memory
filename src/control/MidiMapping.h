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
#include "Common.h"
#include "JsonIO.h"
#include "MidiCommon.h"

class MidiMapping
: public JsonReadable
, public JsonWritable
, public Outputable {
public:
  MidiMapping() {}
  MidiMapping(MidiMappingKey key, std::string path)
  : _key(key)
  , _path(path) {}

  const MidiMappingKey& key() const { return _key; }
  const std::string& path() const { return _path; }

  Json to_json() const override;
  void read_json(const Json& obj) override;

  void output(std::ostream& os) const override;
private:
  MidiMappingKey _key;
  std::string _path;
};

class MidiMappingSet
: public JsonReadable
, public JsonWritable
, public Outputable {
public:
  using Storage = std::vector<MidiMapping>;
  using iterator = Storage::iterator;
  using const_iterator = Storage::const_iterator;

  Json to_json() const override;
  void read_json(const Json& obj) override;
  void output(std::ostream& os) const override;

  void add(MidiMapping&& mapping) { _mappings.push_back(mapping); }

  iterator begin() { return _mappings.begin(); }
  iterator end() { return _mappings.end(); }
  const_iterator begin() const { return _mappings.begin(); }
  const_iterator end() const { return _mappings.end(); }

  std::size_t size() const { return _mappings.size(); }
  bool empty() const { return _mappings.empty(); }
private:
  Storage _mappings;
};

#endif /* MidiMapping_h */
