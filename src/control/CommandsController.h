//
//  CommandsController.h
//

#pragma once

#include <ofxTCommon.h>
#include <unordered_map>
#include "../control/Command.h"
#include "../core/Component.h"

class CommandsController
: public ofxTCommon::NonCopyable
, public ComponentBase {
public:
  using CommandMap = std::unordered_map<std::string, CommandPtr>;

  void setup() override;

  void registerCommand(std::string name,
                       std::string label,
                       CommandFn function,
                       bool supportsButton = true);

  bool perform(const std::string& name, const CommandArgs& args);
  bool perform(const std::string& name);

  CommandMap& commands() { return _commands; }
private:
  CommandMap _commands;
};
