//
//  KeyboardMap.h
//

#pragma once

#include <ofEvents.h>
#include <ofxTCommon.h>
#include <string>
#include <unordered_map>
#include "../control/Command.h"

class KeyboardCommandMapping {
public:
  KeyboardCommandMapping() { }

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

class KeyboardMap
: public ofxTCommon::NonCopyable {
public:
  using Storage = std::unordered_map<int, KeyboardCommandMapping>;

  void registerCommand(int key, KeyboardCommandMapping mapping);

  template<typename ...Args>
  void registerCommand(int key, std::string name, Args&& ...args) {
    registerCommand(key,
                    KeyboardCommandMapping(std::forward<Args>(args)...));
  }

  bool handleKeyPressed(ofKeyEventArgs event);
private:
  Storage _mappings;
};
