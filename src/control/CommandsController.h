//
//  CommandsController.h
//

#pragma once

#include <ofxTCommon.h>
#include <unordered_map>
#include "../control/Command.h"
#include "../control/KeyboardMap.h"
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
                       bool supportsButton = true,
                       int key = 0);

  bool perform(const std::string& name, const CommandArgs& args);
  bool perform(const std::string& name);

  bool handleKeyPressed(ofKeyEventArgs event) {
    return _keyMap.handleKeyPressed(event);
  }

  CommandMap& commands() { return _commands; }
  KeyboardMap& keyMap() { return _keyMap; }
private:
  CommandMap _commands;
  KeyboardMap _keyMap;
};
