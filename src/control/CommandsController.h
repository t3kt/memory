//
//  CommandsController.h
//

#pragma once

#include <ofxTCommon.h>
#include <unordered_map>
#include "../control/Command.h"
#include "../control/KeyboardMap.h"
#include "../core/Component.h"

class CommandsController;

class CommandRegistration {
public:
  CommandRegistration(CommandsController& controller,
                      CommandPtr command)
  : _controller(controller)
  , _command(command) { }

  CommandRegistration& withButton(bool button);
  CommandRegistration& withKeyMapping(int key,
                                      CommandArgs args = CommandArgs());
private:
  CommandsController& _controller;
  CommandPtr _command;
};

class CommandsController
: public ofxTCommon::NonCopyable
, public ComponentBase {
public:
  using CommandMap = std::unordered_map<std::string, CommandPtr>;

  void setup() override;

  CommandRegistration registerCommand(std::string name,
                                      std::string label,
                                      CommandFn function);

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
