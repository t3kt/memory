//
//  Command.cpp
//

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
