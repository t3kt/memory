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
#include <ofxTCommon.h>
#include <ofxTJsonIO.h>
#include <vector>
#include "../app/AppActions.h"
#include "../control/ParamPresets.h"
#include "../control/Params.h"
#include "../core/Component.h"

class Context;
class MemoryAppParameters;
class ParamTransitionSet;

using PresetList = std::vector<std::shared_ptr<ParamPreset>>;

class ParametersState
: public ofxTCommon::NonCopyable
, public ofxTCommon::JsonReadable
, public ofxTCommon::JsonWritable {
public:

  ParametersState(MemoryAppParameters& params)
  : _params(params) { }

  ofJson toJson() const override;
  void readJson(const ofJson& obj) override;

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

  ParametersController(MemoryAppParameters& params,
                       Context& context)
  : _params(params)
  , _state(params)
  , _context(context) { }

  void setup() override;
  void update() override;

  PresetList& presets() { return _state.presets();; }
  const PresetList& presets() const { return _state.presets(); }

  TParamBase* lookupPath(const std::string& path);

  void loadPreset(const ParamPreset& preset);
  void transitionToPreset(const ParamPreset& preset);

  bool performAction(AppAction action) override;
  void load();
  void save();
  void captureNewPreset();
  void resetParams();
  void writeMetadata();
private:
  MemoryAppParameters& _params;
  ParametersState _state;
  bool _isCapturingPreset; // ugly hack...
  Context& _context;
};

#endif /* ParametersController_h */
