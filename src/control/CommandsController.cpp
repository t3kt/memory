//
//  CommandsController.cpp
//

#include <ofLog.h>
#include <ofxTEnums.h>
#include "../app/AppActions.h"
#include "../app/AppSystem.h"
#include "../control/CommandsController.h"

void CommandsController::setup() {
  registerCommand("action", [](Context& context,
                               const CommandArgs& args) {
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
  });
}

void CommandsController::registerCommand(std::string name,
                                         CommandPtr command) {
  _commands[name] = command;
}

void CommandsController::registerCommand(std::string name,
                                         CommandFn function) {
  registerCommand(name, Command::of(function));
}

bool CommandsController::perform(const std::string &name,
                                 const CommandArgs &args) const {
  auto iter = _commands.find(name);
  if (iter == _commands.end()) {
    return false;
  }
  return iter->second->perform(_context, args);
}

bool CommandsController::perform(const std::string &name) const {
  CommandArgs args;
  return perform(name, args);
}
