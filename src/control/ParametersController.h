//
//  ParametersController.h
//  memory
//
//  Created by tekt on 10/12/16.
//
//

#ifndef ParametersController_h
#define ParametersController_h

#include <memory>
#include <vector>
#include "../app/AppActions.h"
#include "../control/ParamPresets.h"
#include "../control/Params.h"
#include "../core/Common.h"
#include "../core/Component.h"
#include "../core/JsonIO.h"

class MemoryAppParameters;

class ParametersState
: public NonCopyable
, public JsonReadable
, public JsonWritable {
public:
  using PresetList = std::vector<std::shared_ptr<ParamPreset>>;

  ParametersState(MemoryAppParameters& params)
  : _params(params) { }

  Json to_json() const override;
  void read_json(const Json& obj) override;

  PresetList& presets() { return _presets; }
  const PresetList& presets() const { return _presets; }

  void addPreset(std::shared_ptr<ParamPreset> preset) {
    _presets.push_back(preset);
  }
private:
  MemoryAppParameters& _params;
  PresetList _presets;
};

class ParametersController
: public NonCopyable
, public ComponentBase
, public AppActionHandler {
public:
  using State = ParametersState;

  ParametersController(MemoryAppParameters& params)
  : _params(params)
  , _state(params) { }

  void setup() override;
  void update() override;

  bool performAction(AppAction action) override;
  void load();
  void save();
  void captureNewPreset();
private:
  MemoryAppParameters& _params;
  ParametersState _state;
};

#endif /* ParametersController_h */
