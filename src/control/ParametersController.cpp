//
//  ParametersController.cpp
//  memory
//
//  Created by tekt on 10/12/16.
//
//

#include "../app/AppParameters.h"
#include "../app/AppSystem.h"
#include "../control/ParametersController.h"
#include "../control/ParamTransition.h"
#include "../core/Actions.h"

Json ParametersState::to_json() const {
  auto obj = Json::object {
    {"params", _params},
  };
  if (!_presets.empty()) {
    auto presetsArr = Json::array();
    for (const auto& preset : _presets) {
      presetsArr.push_back(preset->to_json());
    }
    obj["presets"] = presetsArr;
  }
  return obj;
}

void ParametersState::read_json(const Json &obj) {
  JsonUtil::assertHasType(obj, Json::OBJECT);
  const Json *paramVals;
  if (obj["params"].is_null()) {
    // parsing it as old format that only contains raw params
    paramVals = &obj;
  } else {
    paramVals = &(obj["params"]);
    const auto& presetsArr = obj["presets"];
    if (presetsArr.is_array()) {
      _params.clear();
      for (const auto& presetObj : presetsArr.array_items()) {
        auto preset = std::make_shared<ParamPreset>();
        preset->read_json(presetObj);
        preset->stripUnsupportedParams(_params);
        _presets.push_back(preset);
      }
    }
  }
  if (!paramVals->is_null()) {
    JsonUtil::assertHasType(*paramVals, Json::OBJECT);
    _params.read_json(*paramVals);
  } else {
    AppSystem::get().log().app().logWarning("Unable to find param values in settings json");
  }
}

void ParametersController::setup() {
  load();
}

void ParametersController::update() {
  //...
}

bool ParametersController::performAction(AppAction action) {
  switch (action) {
    case AppAction::LOAD_SETTINGS:
      load();
      return true;
    case AppAction::SAVE_SETTINGS:
      save();
      return true;
    case AppAction::CAPTURE_PRESET:
      captureNewPreset();
      return true;
    case AppAction::RESET_PARAMS:
      resetParams();
      return true;
    default:
      return false;
  }
}

void ParametersController::resetParams() {
  AppSystem::get().log().app().logNotice("Resetting parameters...");
  _params.resetToDefault();
}

void ParametersController::load() {
  AppSystem::get().log().app().logNotice("Reading JSON settings...");

  AppSystem::get().doWhilePaused([&]() {
    _state.readFromFile("settings.json");
    //...
    return true;
  });
  AppSystem::get().log().app().logNotice([&](ofLog& log) {
    log << ".. read from JSON finished\n\t" << _params;
  });
}

void ParametersController::save() {
  AppSystem::get().log().app().logNotice("Writing JSON settings...");
  AppSystem::get().doWhilePaused([&]() {
    _state.writeToFile("settings.json");
    return true;
  });
  AppSystem::get().log().app().logNotice(".. write to JSON finished");
}

void ParametersController::captureNewPreset() {
  if (_isCapturingPreset) {
    // see https://github.com/t3kt/memory#15
    AppSystem::get().log().app().logWarning("Already capturing preset... it's that annoying duplicate action bug...");
    return;
  }
  _isCapturingPreset = true;
  AppSystem::get().log().app().logNotice("Capturing preset...");
  auto defaultName =
    "preset " + ofToString(_state.presets().size() + 1);
  auto name = AppSystem::promptForText("Preset name",
                                       defaultName);
  if (name.empty()) {
    AppSystem::get().log().app().logNotice("Not creating preset");
    return;
  }
  auto preset = std::make_shared<ParamPreset>();
  preset->setName(name);
  preset->captureParams(_params);
  _state.addPreset(preset);

  AppSystem::get().log().app()
  .logNotice("Captured preset: '" + name + "'");
  _isCapturingPreset = false;
}

void ParametersController::loadPreset(const ParamPreset &preset) {
  AppSystem::get().log().app().logNotice("Loading preset...");
  preset.applyToParams(_params);
}

void
ParametersController::transitionToPreset(const ParamPreset &preset) {
  AppSystem::get().log().app().logNotice("Transitioning to preset...");
  auto transitions = std::make_shared<ParamTransitionSet>();
  transitions->loadCurrentToPreset(_params, preset);
  auto action = transitions->createApplyAction(2, _context);
  AppSystem::get().actions().addContinuous(action);
}
