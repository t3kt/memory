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

Json ParametersState::to_json() const {
  auto obj = Json::object {
    {"params", _params},
  };
  //...
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
    default:
      return false;
  }
}

void ParametersController::load() {
  AppSystem::get().log().app().logNotice("Reading JSON settings...");
  _state.readFromFile("settings.json");
  AppSystem::get().log().app().logNotice([&](ofLog& log) {
    log << ".. read from JSON finished\n\t" << _params;
  });
  //...
}

void ParametersController::save() {
  AppSystem::get().log().app().logNotice("Writing JSON settings...");
  _state.writeToFile("settings.json");
  AppSystem::get().log().app().logNotice(".. write to JSON finished");
}
