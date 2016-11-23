//
//  ParametersController.cpp
//  memory
//
//  Created by tekt on 10/12/16.
//
//

#include <ofUtils.h>
#include "../app/AppParameters.h"
#include "../app/AppSystem.h"
#include "../app/SimulationApp.h"
#include "../control/CommandsController.h"
#include "../control/ParametersController.h"
#include "../control/ParamTransition.h"
#include "../core/Actions.h"

using namespace ofxTCommon;

ofJson ParametersState::toJson() const {
  ofJson obj = {
    {"params", _params.toJson()},
  };
  if (!_presets.empty()) {
    auto presetsArr = ofJson::array();
    for (const auto& preset : _presets) {
      presetsArr.push_back(preset->toJson());
    }
    obj["presets"] = presetsArr;
  }
  return obj;
}

void ParametersState::readJson(const ofJson &obj) {
  JsonUtil::assertIsObject(obj);
  const ofJson *paramVals;
  if (obj.count("params") == 0 || obj["params"].is_null()) {
    // parsing it as old format that only contains raw params
    paramVals = &obj;
  } else {
    paramVals = &(obj["params"]);
    if (obj.count("presets") != 0) {
      const auto& presetsArr = obj["presets"];
      if (presetsArr.is_array()) {
        _params.clear();
        for (const auto& presetObj : presetsArr) {
          auto preset = std::make_shared<ParamPreset>();
          preset->readJson(presetObj);
          if (preset->name().empty()) {
            preset->setName("preset " + ofToString(_presets.size()));
          }
          preset->stripUnsupportedParams(_params);
          _presets.push_back(preset);
        }
      }
    }
  }
  if (!paramVals->is_null()) {
    JsonUtil::assertIsObject(*paramVals);
    _params.readJson(*paramVals);
  } else {
    AppSystem::get().log().app().logWarning("Unable to find param values in settings json");
  }
}

void ParametersController::setup() {
  writeMetadata();
  load();
  AppSystem::get().commands()
  .registerCommand("capturePreset", "Capture Preset", [&](const CommandArgs&) {
    captureNewPreset();
    return true;
  })
  .withButton(true);
  AppSystem::get().commands()
  .registerCommand("resetParams", "Reset Params", [&](const CommandArgs&) {
    resetParams();
    return true;
  })
  .withButton(true);
  AppSystem::get().commands()
  .registerCommand("loadSettings", "Load Settings", [&](const CommandArgs&) {
    load();
    return true;
  })
  .withButton(true)
  .withKeyMapping('r');
  AppSystem::get().commands()
  .registerCommand("saveSettings", "Save Settings", [&](const CommandArgs&) {
    save();
    return true;
  })
  .withButton(true)
  .withKeyMapping('w');
}

void ParametersController::update() {
  //...
}

void ParametersController::resetParams() {
  AppSystem::get().log().app().logNotice("Resetting parameters...");
  _params.resetToDefault();
}

TParamBase* ParametersController::lookupPath(const std::string &path) {
  return _params.lookupPath(path);
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
    _state.writeJsonTo("settings.json");
    return true;
  });
  AppSystem::get().log().app().logNotice(".. write to JSON finished");
}

void ParametersController::writeMetadata() {
  AppSystem::get().log().app().logNotice("Writing JSON metadata...");
  AppSystem::get().doWhilePaused([&]() {
    auto meta = _params.toJsonMetadata();
    ofxTCommon::JsonUtil::writeJsonToFile("param-meta.json", meta);
    return true;
  });
  AppSystem::get().log().app().logNotice(".. write metadata JSON finished");
}

void ParametersController::captureNewPreset() {
  if (_isCapturingPreset) {
    // see https://github.com/t3kt/memory#15
    AppSystem::get().log().app().logWarning("Already capturing preset... it's that annoying duplicate action bug...");
    return;
  }
  _isCapturingPreset = true;
  AppSystem::get().log().app().logNotice("Capturing preset...");
  auto name = AppSystem::promptForText("Preset name");
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

  AppSystem::get().simulation().gui().updatePresetButtons();
}

void ParametersController::loadPreset(const ParamPreset &preset) {
  AppSystem::get().log().app().logNotice("Loading preset...");
  preset.applyToParams(_params);
}

void
ParametersController::transitionToPreset(const ParamPreset &preset) {
  if (_context.activeTransition) {
    AppSystem::get().log().app().logNotice("Aborting active transition: " + _context.activeTransition->name());
    _context.activeTransition->abortApplyAction();
    _context.activeTransition.reset();
  }
  AppSystem::get().log().app().logNotice("Transitioning to preset " + preset.name() + "...");
  auto transitions = std::make_shared<ParamTransitionSet>();
  transitions->loadCurrentToPreset(_params, preset);
  transitions->setName("to preset '" + preset.name() + "'");
  auto action = transitions->createApplyAction(5, _context);
  _context.activeTransition = transitions;
  ActionFinishCallback onFinish = [&]() {
    AppSystem::get().log().app().logNotice("Finished transition: " + _context.activeTransition->name());
    _context.activeTransition.reset();
  };
  AppSystem::get().actions().addContinuous(action,
                                           onFinish);
}
