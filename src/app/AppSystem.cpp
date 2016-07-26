//
//  AppSystem.cpp
//  memory
//
//  Created by tekt on 7/14/16.
//
//

#include <map>
#include <ofLog.h>
#include "AppSystem.h"
#include "ControlApp.h"
#include "SimulationApp.h"

const int CONTROLS_WIDTH = 250;

static std::unique_ptr<AppSystem> instance;

void AppSystem::initialize() {
  instance = std::make_unique<AppSystem>();
}

AppSystem& AppSystem::get() {
  return *instance;
}

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

bool AppSystem::handleKeyPressed(ofKeyEventArgs &event) {
  auto iter = KEY_TO_ACTION.find(event.key);
  if (iter == KEY_TO_ACTION.end()) {
    return false;
  }
  return performAction(iter->second);
}

void AppSystem::setup() {
  ofGLFWWindowSettings simWinSettings;
  simWinSettings.width = 1100;
  simWinSettings.height = 800;
  simWinSettings.resizable = false;
  simWinSettings.setPosition(ofVec3f(CONTROLS_WIDTH + 5, 0));
  _simulationWindow =
  std::static_pointer_cast<ofAppGLFWWindow>(ofCreateWindow(simWinSettings));

  ofGLFWWindowSettings ctrlWinSettings;
  ctrlWinSettings.width = CONTROLS_WIDTH;
  ctrlWinSettings.height = 800;
  ctrlWinSettings.resizable = true;
  ctrlWinSettings.setPosition(ofVec3f(0, 0));
  _controlWindow =
  std::static_pointer_cast<ofAppGLFWWindow>(ofCreateWindow(ctrlWinSettings));

  _simulationApp = std::make_shared<SimulationApp>(_appParams,
                                                   _context,
                                                   _simulationWindow);

  _controlApp = std::make_shared<ControlApp>(_appParams);
}

void AppSystem::main() {
  setup();

  ofRunApp(_controlWindow, _controlApp);
  ofRunApp(_simulationWindow, _simulationApp);
  ofRunMainLoop();
}

bool AppSystem::performAction(AppAction action) {
  auto args = AppActionEventArgs(action);
  bool handled = appActionTriggered.notifyListenersUntilHandled(args);
  if (!handled) {
    _log.app().logWarning("App action not handled: " + AppActionType.toString(action));
  }
  return handled;
}
