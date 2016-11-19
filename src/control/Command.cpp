//
//  Command.cpp
//

#include <ofLog.h>
#include "../control/Command.h"

class FnCommand
: public Command {
public:
  FnCommand(CommandFn function)
  : _function(function) { }

  bool perform(Context& context,
               const CommandArgs& args) override {
    return _function(context, args);
  }
private:
  CommandFn _function;
};

/* static */
CommandPtr Command::of(CommandFn fn) {
  return std::make_shared<FnCommand>(fn);
}

bool TestCommand::perform(Context &context, const CommandArgs &args) {
  if (args.empty()) {
    return false;
  }
  if (args[0].type() != typeid(std::string)) {
    return false;
  }
  ofLogNotice() << "Hello TestCommand: " << args.get<std::string>(0);
}

bool CommandSet::perform(const std::string &name,
                         Context &context,
                         const CommandArgs &args) const {
  auto iter = _commands.find(name);
  if (iter == _commands.end()) {
    return false;
  }
  return iter->second->perform(context, args);
}
