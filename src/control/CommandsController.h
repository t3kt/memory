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

// A record of the registration of a command, which provides a
// fluent-style interface for registering commands with UI and/or
// keyboard mappings.
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

// Controller which manages commands and invokes them in response
// to UI and/or keyboard input.
class CommandsController
: public ofxTCommon::NonCopyable
, public ComponentBase {
public:
  using CommandMap = std::unordered_map<std::string, CommandPtr>;

  CommandRegistration registerCommand(CommandPtr command);

  CommandRegistration registerCommand(std::string name,
                                      std::string label,
                                      CommandFn function);

  CommandRegistration registerCommand(std::string name,
                                      std::string label,
                                      std::function<bool()> function);

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
