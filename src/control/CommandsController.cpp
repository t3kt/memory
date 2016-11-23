//
//  CommandsController.cpp
//

#include <ofLog.h>
#include <ofxTEnums.h>
#include "../app/AppSystem.h"
#include "../control/CommandsController.h"

CommandRegistration& CommandRegistration::withButton(bool button) {
  _command->setSupportsButton(button);
  return *this;
}

CommandRegistration& CommandRegistration::withKeyMapping(int key, CommandArgs args) {
  _controller.keyMap().registerCommand(key, KeyboardCommandMapping(_command->name(), args));
  return *this;
}

CommandRegistration
CommandsController::registerCommand(CommandPtr command) {
  _commands[command->name()] = command;
  return CommandRegistration(*this, command);
}

CommandRegistration
CommandsController::registerCommand(std::string name,
                                    std::string label,
                                    CommandFn function) {
  auto command = _commands[name] = Command::of(name, label, function);
  return CommandRegistration(*this, command);
}

CommandRegistration
CommandsController::registerCommand(std::string name, std::string label, std::function<bool ()> function) {
  return registerCommand(name, label, [&](const CommandArgs& args) {
    return function();
  });
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
