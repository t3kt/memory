//
//  Command.cpp
//

#include <ofLog.h>
#include "../control/Command.h"

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
