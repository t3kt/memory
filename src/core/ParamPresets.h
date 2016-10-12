//
//  ParamPresets.h
//  memory
//
//  Created by tekt on 10/11/16.
//
//

#ifndef ParamPresets_h
#define ParamPresets_h

#include "../core/JsonIO.h"
#include "../core/Params.h"

class ParamPreset
: public JsonReadable
, public JsonWritable {
public:

  void read_json(const Json& obj) override;
  Json to_json() const override;
private:
  std::string _name;
  Json _values;
};

#endif /* ParamPresets_h */
