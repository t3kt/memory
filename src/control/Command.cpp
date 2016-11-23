//
//  Command.cpp
//

#include "../control/Command.h"

class FnCommand
: public Command {
public:
  FnCommand(std::string name,
            std::string label,
            CommandFn performFunc)
  : Command(name, label)
  , _perform(performFunc) { }

  bool perform(const CommandArgs& args) override {
    return _perform(args);
  }
private:
  CommandFn _perform;
};

/* static */
CommandPtr Command::of(std::string name,
                       std::string label,
                       CommandFn performFunc) {
  return std::make_shared<FnCommand>(name, label, performFunc);
}
