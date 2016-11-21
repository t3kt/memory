//
//  CommandsController.cpp
//

#include <ofLog.h>
#include <ofxTEnums.h>
#include "../app/AppActions.h"
#include "../app/AppSystem.h"
#include "../control/CommandsController.h"

void CommandsController::setup() {
  registerCommand("action", "Perform Action",
                  [](const CommandArgs& args) {
    if (args.empty() || args[0].type() != typeid(std::string)) {
      return false;
    }
    std::string name = Poco::AnyCast<std::string>(args[0]);
    AppAction action;
    if (!ofxTCommon::getEnumInfo<AppAction>().tryParseString(name,
                                                             &action)) {
      return false;
    }
    return AppSystem::get().performAction(action);
  }, false);
}

void CommandsController::registerCommand(std::string name,
                                         std::string label,
                                         CommandFn function,
                                         bool supportsButton,
                                         int key) {
  _commands[name] =
  std::make_shared<Command>(name, label, function, supportsButton);

  if (key > 0) {
    _keyMap.registerCommand(key, name);
  }
}

bool CommandsController::perform(const std::string &name,
                                 const CommandArgs &args) {
  auto iter = _commands.find(name);
  if (iter == _commands.end()) {
    return false;
  }
  return iter->second->perform(args);
}

bool CommandsController::perform(const std::string &name) {
  CommandArgs args;
  return perform(name, args);
}
