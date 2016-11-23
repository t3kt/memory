//
//  ParametersController.h
//

#pragma once

#include <memory>
#include <ofxTCommon.h>
#include <ofxTJsonIO.h>
#include <vector>
#include "../control/ParamPresets.h"
#include "../control/Params.h"
#include "../core/Component.h"

class Context;
class MemoryAppParameters;
class ParamTransitionSet;

using PresetList = std::vector<PresetPtr>;

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

  void addPreset(PresetPtr preset) {
    _presets.push_back(preset);
  }

  PresetPtr getPreset(const std::string& name);
private:
  MemoryAppParameters& _params;
  PresetList _presets;
};

class ParametersController
: public ofxTCommon::NonCopyable
, public ComponentBase {
public:
  using State = ParametersState;

  ParametersController(MemoryAppParameters& params,
                       Context& context)
  : _params(params)
  , _state(params)
  , _context(context) { }

  void setup() override;

  PresetList& presets() { return _state.presets();; }
  const PresetList& presets() const { return _state.presets(); }

  TParamBase* lookupPath(const std::string& path);

  bool loadPreset(std::string presetName = "");
  bool transitionToPreset(std::string presetName = "");

  void load(std::string filename = "");
  void save(std::string filename = "");
  void captureNewPreset(std::string presetName = "");
  void resetParams();
  void writeMetadata();
private:
  MemoryAppParameters& _params;
  ParametersState _state;
  bool _isCapturingPreset; // ugly hack...
  Context& _context;
};

