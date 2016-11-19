//
//  CommandsController.h
//

#pragma once

#include <ofxTCommon.h>
#include <unordered_map>
#include "../control/Command.h"
#include "../core/Component.h"

class Context;

class CommandsController
: public ofxTCommon::NonCopyable
, public ComponentBase {
public:
  using CommandMap = std::unordered_map<std::string, CommandPtr>;

  CommandsController(Context& context)
  : _context(context) { }

  void setup() override;

  void registerCommand(std::string name, CommandPtr command);
  void registerCommand(std::string name, CommandFn function);

  template<typename T, typename ...Args>
  std::shared_ptr<T> registerCommand(std::string name, Args&& ...args) {
    auto command = std::make_shared<T>(std::forward<Args>(args)...);
    registerCommand(name, command);
    return command;
  }

  bool perform(const std::string& name, const CommandArgs& args) const;
  bool perform(const std::string& name) const;
private:
  CommandMap _commands;
  Context& _context;
};
