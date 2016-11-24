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

PresetPtr ParametersState::getPreset(const std::string &name) {
  for (auto& preset : _presets) {
    if (preset->name() == name) {
      return preset;
    }
  }
  return nullptr;
}

void ParametersController::setup() {
  writeMetadata();
  load();
  AppSystem::get().commands()
  .registerCommand("capturePreset", "Capture Preset", [&](const CommandArgs& args) {
    captureNewPreset(args.getOrDefault<std::string>(0));
    return true;
  })
  .withButton(true);
  AppSystem::get().commands()
  .registerCommand("loadPreset", "Load Preset", [&](const CommandArgs& args) {
    return loadPreset(args.getOrDefault<std::string>(0));
  })
  .withButton(true);
  AppSystem::get().commands()
  .registerCommand("transitionPreset", "Transition to Preset", [&](const CommandArgs& args) {
    return transitionToPreset(args.getOrDefault<std::string>(0));
  })
  .withButton(true);
  AppSystem::get().commands()
  .registerCommand("resetParams", "Reset Params", [&](const CommandArgs&) {
    resetParams();
    return true;
  })
  .withButton(true);
  AppSystem::get().commands()
  .registerCommand("loadSettings", "Load Settings", [&](const CommandArgs& args) {
    load(args.getOrDefault<std::string>(0));
    return true;
  })
  .withButton(true)
  .withKeyMapping('r');
  AppSystem::get().commands()
  .registerCommand("saveSettings", "Save Settings", [&](const CommandArgs& args) {
    save(args.getOrDefault<std::string>(0));
    return true;
  })
  .withButton(true)
  .withKeyMapping('w');
  AppSystem::get().commands()
  .registerCommand("setParams", "Set Params From JSON", [&](const CommandArgs& args) {
    if (!args.hasArgType<std::string>(0)) {
      return false;
    }
    std::string path;
    std::string json;
    if (args.hasArgType<std::string>(1)) {
      path = args.get<std::string>(0);
      json = args.get<std::string>(1);
    } else {
      path = "";
      json = args.get<std::string>(0);
    }
    return setFromJson(path, json);
  });
}

void ParametersController::resetParams() {
  AppSystem::get().log().app().logNotice("Resetting parameters...");
  _params.resetToDefault();
}

TParamBase* ParametersController::lookupPath(const std::string &path) {
  return _params.lookupPath(path);
}

void ParametersController::load(std::string filename) {
  if (filename.empty()) {
    filename = "settings.json";
  }
  AppSystem::get().log().app().logNotice("Reading JSON settings from " + filename);
  AppSystem::get().doWhilePaused([&]() {
    _state.readFromFile(filename);
    return true;
  });
  AppSystem::get().log().app().logNotice([&](ofLog& log) {
    log << ".. read from JSON finished\n\t" << _params;
  });
}

void ParametersController::save(std::string filename) {
  if (filename.empty()) {
    filename = "settings.json";
  }
  AppSystem::get().log().app().logNotice("Writing JSON settings to " + filename);
  AppSystem::get().doWhilePaused([&]() {
    _state.writeJsonTo(filename);
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

void ParametersController::captureNewPreset(std::string presetName) {
  if (_isCapturingPreset) {
    // see https://github.com/t3kt/memory#15
    AppSystem::get().log().app().logWarning("Already capturing preset... it's that annoying duplicate action bug...");
    return;
  }
  _isCapturingPreset = true;
  if (presetName.empty()) {
    presetName = AppSystem::promptForText("Preset name");
    if (presetName.empty()) {
      AppSystem::get().log().app().logNotice("Not creating preset");
      return;
    }
  }
  AppSystem::get().log().app().logNotice("Capturing preset " + presetName);
  auto preset = std::make_shared<ParamPreset>();
  preset->setName(presetName);
  preset->captureParams(_params);
  _state.addPreset(preset);

  AppSystem::get().log().app()
  .logNotice("Captured preset: '" + presetName + "'");
  _isCapturingPreset = false;

  AppSystem::get().simulation().gui().updatePresetButtons();
}

bool ParametersController::loadPreset(std::string presetName) {
  if (presetName.empty()) {
    presetName = AppSystem::promptForText("Preset to load");
    if (presetName.empty()) {
      return false;
    }
  }
  auto preset = _state.getPreset(presetName);
  if (!preset) {
    AppSystem::get().log().app().logWarning("Preset not found: " + presetName);
    return false;
  }
  AppSystem::get().log().app().logNotice("Loading preset " + presetName);
  preset->applyToParams(_params);
  return true;
}

bool ParametersController::transitionToPreset(std::string presetName) {
  if (presetName.empty()) {
    presetName = AppSystem::promptForText("Preset to transition to");
    if (presetName.empty()) {
      return false;
    }
  }

  auto preset = _state.getPreset(presetName);
  if (!preset) {
    AppSystem::get().log().app().logWarning("Preset not found: " + presetName);
    return false;
  }
  if (_context.activeTransition) {
    AppSystem::get().log().app().logNotice("Aborting active transition: " + _context.activeTransition->name());
    _context.activeTransition->abortApplyAction();
    _context.activeTransition.reset();
  }

  AppSystem::get().log().app().logNotice("Transitioning to preset " + presetName + "...");
  auto transitions = std::make_shared<ParamTransitionSet>();
  transitions->loadCurrentToPreset(_params, *preset);
  transitions->setName("to preset '" + presetName + "'");
  auto action = transitions->createApplyAction(5, _context);
  _context.activeTransition = transitions;
  ActionFinishCallback onFinish = [&]() {
    AppSystem::get().log().app().logNotice("Finished transition: " + _context.activeTransition->name());
    _context.activeTransition.reset();
  };
  AppSystem::get().actions().addContinuous(action,
                                           onFinish);
  return true;
}

bool ParametersController::setFromJson(const std::string& path, const std::string& json) {
  try {
    auto obj = ofJson::parse(json);
    return setFromJson(path, obj);
  }catch(std::exception & e){
    ofLogError("setFromJson") << "error loading json: " << e.what();
    return false;
  }catch(...){
    ofLogError("setFromJson") << "error loading json";
    return false;
  }
  return false;
}

bool ParametersController::setFromJson(const std::string& path, const ofJson& obj) {
  if (path.empty()) {
    _params.readJson(obj);
    return true;
  }
  auto params = _params.lookupPath(path);
  if (!params) {
    return false;
  }
  params->readJson(obj);
  return true;
}
