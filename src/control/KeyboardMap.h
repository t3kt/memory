//
//  KeyboardMap.h
//

#pragma once

#include <ofEvents.h>
#include <ofxTCommon.h>
#include <string>
#include <unordered_map>
#include "../control/Command.h"

// Settings for a binding of a command to a keyboard shortcut.
class KeyboardCommandMapping {
public:
  KeyboardCommandMapping(std::string name,
                         CommandArgs args)
  : _name(name)
  , _args(args) { }

  KeyboardCommandMapping(std::string name)
  : _name(name) { }

  const std::string& name() const { return _name; }
  const CommandArgs& args() const { return _args; }
private:
  std::string _name;
  CommandArgs _args;
};

// Map of keyboard shortcuts to commands.
class KeyboardMap
: public ofxTCommon::NonCopyable {
public:
  using Storage = std::unordered_map<int, KeyboardCommandMapping>;

  void registerCommand(int key, KeyboardCommandMapping mapping);
  void registerCommand(int key, std::string name) {
    registerCommand(key, KeyboardCommandMapping(name));
  }
  void registerCommand(int key, std::string name, CommandArgs args) {
    registerCommand(key, KeyboardCommandMapping(name, args));
  }

  bool handleKeyPressed(ofKeyEventArgs event);
private:
  Storage _mappings;
};
