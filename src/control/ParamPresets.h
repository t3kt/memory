//
//  ParamPresets.h
//  memory
//
//  Created by tekt on 10/11/16.
//
//

#ifndef ParamPresets_h
#define ParamPresets_h

#include "../control/Params.h"
#include "../core/JsonIO.h"

class ParamPreset
: public JsonReadable
, public JsonWritable {
public:

  const std::string& name() const { return _name; }
  void setName(const std::string& name) { _name = name; }

  void read_json(const Json& obj) override;
  Json to_json() const override;

  void captureParams(const Params& params);
  void applyToParams(Params& params) const;
private:
  std::string _name;
  Json _values;
};

#endif /* ParamPresets_h */
