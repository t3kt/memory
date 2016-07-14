//
//  ControlApp.cpp
//  memory
//
//  Created by tekt on 7/10/16.
//
//

#include "ControlApp.h"
#include "SimulationApp.h"
#include <map>

static std::map<int, AppAction> KEY_TO_ACTION = {
  {'h', AppAction::RESET_CAMERA},
  {'l', AppAction::TOGGLE_LOGGING},
  {' ', AppAction::TOGGLE_CLOCK_STATE},
  {'9', AppAction::SPAWN_FEW_OCCURRENCES},
  {'0', AppAction::SPAWN_FEW_OBSERVERS},
  {'(', AppAction::SPAWN_MANY_OCCURRENCES},
  {')', AppAction::SPAWN_MANY_OBSERVERS},
  {'-', AppAction::KILL_FEW_OBSERVERS},
  {'_', AppAction::KILL_MANY_OBSERVERS},
  {'r', AppAction::LOAD_SETTINGS},
  {'w', AppAction::SAVE_SETTINGS},
  {'x', AppAction::STOP_ALL_ENTITIES},
  {'p', AppAction::TOGGLE_SHOW_PHYSICS},
  {'b', AppAction::TOGGLE_SHOW_BOUNDS},
};

void ControlApp::attachSimulation(std::shared_ptr<SimulationApp> simulation) {
  _simulation = simulation;
}

void ControlApp::setup() {
  _appParams.core.debug.loggingEnabledChanged += [this]() {
    updateLogLevel();
  };

  updateLogLevel();
  loadSettings();

  _gui = std::make_shared<AppGui>(_appParams, *this);
}

void ControlApp::updateLogLevel() {
  ofSetLogLevel(_appParams.core.debug.loggingEnabled()
                ? OF_LOG_VERBOSE : OF_LOG_ERROR);
}

void ControlApp::draw() {
  _gui->draw();
}

void ControlApp::keyPressed(int key) {
  auto iter = KEY_TO_ACTION.find(key);
  if (iter == KEY_TO_ACTION.end()) {
    return;
  }
  performAction(iter->second);
}

void ControlApp::performAction(AppAction action) {
  switch (action) {
    case AppAction::LOAD_SETTINGS:
      loadSettings();
      break;
    case AppAction::SAVE_SETTINGS:
      saveSettings();
      break;
    default:
      _simulation->performAction(action);
  }
}

void ControlApp::loadSettings() {
  ofLogNotice() << "Reading JSON settings...";
  _appParams.readFromFile("settings.json");
  ofLogNotice() << ".. read from JSON finished\n\t" << _appParams;
}

void ControlApp::saveSettings() {
  ofLogNotice() << "Writing JSON settings...";
  _appParams.writeToFile("settings.json");
  ofLogNotice() << ".. write to JSON finished";
}
