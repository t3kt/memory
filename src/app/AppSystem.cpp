//
//  AppSystem.cpp
//
//

#include <map>
#include <ofSystemUtils.h>
#include "../app/AppSystem.h"
#include "../app/SimulationApp.h"
#include "../core/TimingController.h"

static std::unique_ptr<AppSystem> instance;

void AppSystem::initialize() {
  instance = std::make_unique<AppSystem>();
}

AppSystem& AppSystem::get() {
  return *instance;
}

static std::map<int, AppAction> KEY_TO_ACTION = {
  {'0', AppAction::SPAWN_FEW_OBSERVERS},
  {')', AppAction::SPAWN_MANY_OBSERVERS},
  {'9', AppAction::SPAWN_FEW_OCCURRENCES},
  {'(', AppAction::SPAWN_MANY_OCCURRENCES},
  {'|', AppAction::SPAWN_LOAD_TEST_ENTITIES},
  {'n', AppAction::SPAWN_OBSERVER_NAVIGATOR},
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
  simWinSettings.width = 1400;
  simWinSettings.height = 800;
  simWinSettings.resizable = false;
  simWinSettings.setPosition(glm::vec2(0, 0));
  _simulationWindow =
  std::static_pointer_cast<ofAppGLFWWindow>(ofCreateWindow(simWinSettings));

  _simulationApp = std::make_shared<SimulationApp>(_appParams,
                                                   _context,
                                                   _simulationWindow);
}

ActionsController& AppSystem::actions() {
  return _simulationApp->actions();
}

CommandsController& AppSystem::commands() {
  return _simulationApp->commands();
}

void AppSystem::main() {
  setup();

  ofRunApp(_simulationWindow, _simulationApp);
  ofRunMainLoop();
}

bool AppSystem::performAction(AppAction action) {
  _log.app().logVerbose("Performing AppAction: " + enumToString(action) + "...");
  auto args = AppActionEventArgs(action);
  bool handled = appActionTriggered.notifyListenersUntilHandled(args);
  if (!handled) {
    _log.app().logWarning("App action not handled: " + enumToString(action));
  }
  return handled;
}

bool AppSystem::doWhilePaused(std::function<bool(void)> action) {
  return _simulationApp->timing().doWhilePaused(action);
}

bool AppSystem::performFileSaveAction(FileAction action,
                                      std::string messageName,
                                      std::string defaultName) {
  return doWhilePaused([&]() {
    auto result = ofSystemSaveDialog(defaultName, messageName);
    if (!result.bSuccess) {
      return false;
    }
    return action(result);
  });
}

bool AppSystem::performFileLoadAction(FileAction action,
                                      std::string windowTitle,
                                      std::string defaultPath) {
  return doWhilePaused([&]() {
    auto result = ofSystemLoadDialog(windowTitle,
                                     false,
                                     defaultPath);
    if (!result.bSuccess) {
      return false;
    }
    return action(result);
  });
}

/*static */
std::string AppSystem::promptForText(std::string message,
                                     std::string initialText) {
  return ofSystemTextBoxDialog(message,
                               initialText);
}
