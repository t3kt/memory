//
//  MidiMapping.h
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#ifndef MidiMapping_h
#define MidiMapping_h

#include <ofxTJsonIO.h>
#include <string>
#include "../core/Common.h"
#include "../control/MappingSet.h"
#include "../control/MidiCommon.h"

class MidiMapping
: public ofxTCommon::JsonReadable
, public ofxTCommon::JsonWritable
, public Outputable {
public:
  MidiMapping() {}
  MidiMapping(MidiMappingKey key, std::string path)
  : _key(key)
  , _path(path) {}

  const MidiMappingKey& key() const { return _key; }
  const std::string& path() const { return _path; }

  ofJson toJson() const override;
  void readJson(const ofJson& obj) override;

  std::string typeName() const override { return "MidiMapping"; }
protected:
  void outputFields(std::ostream& os) const override;
private:
  MidiMappingKey _key;
  std::string _path;
};

using MidiMappingSet = MappingSet<MidiMapping>;

#endif /* MidiMapping_h */
