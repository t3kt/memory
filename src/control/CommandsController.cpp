//
//  CommandsController.cpp
//

#include <ofLog.h>
#include "../control/CommandsController.h"

class TestCommand
: public Command {
public:
  bool perform(Context& context,
               const CommandArgs& args) override {
    if (args.empty()) {
      return false;
    }
    if (args[0].type() != typeid(std::string)) {
      return false;
    }
    ofLogNotice() << "Hello TestCommand: " << args.get<std::string>(0);
  }
};

void CommandsController::setup() {
  registerCommand<TestCommand>("test");
  //...
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
