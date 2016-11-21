//
//  Command.h
//  memory
//

#pragma once

#include <cstdint>
#include <functional>
#include <initializer_list>
#include <memory>
#include <ofxOscMessage.h>
#include <ofxTCommon.h>
#include <Poco/Any.h>
#include <stdexcept>
#include <string>

using CommandArg = Poco::Any;

class CommandArgs {
public:
  using Arg = CommandArg;
  using Storage = std::vector<Arg>;

  CommandArgs() { }

  CommandArgs(std::initializer_list<Arg> args)
  : _args(args) { }

  CommandArgs& add(Arg arg) {
    _args.push_back(arg);
    return *this;
  }

  Arg operator[](std::size_t i) const {
    return _args[i];
  }

  std::size_t size() const { return _args.size(); }
  bool empty() const { return _args.empty(); }

  template<typename T>
  T get(std::size_t i) const {
    return Poco::AnyCast<T>(_args[i]);
  }

private:
  Storage _args;
};

using CommandFn = std::function<bool(const CommandArgs&)>;

class Command
: public ofxTCommon::NonCopyable {
public:
  Command(std::string name,
          std::string label,
          CommandFn perfFunc)
  : _name(name)
  , _label(label)
  , _perform(perfFunc)
  , _supportsButton(false) { }

  const std::string& name() const { return _name; }
  const std::string& label() const { return _label; }
  bool supportsButton() const { return _supportsButton; }

  void setSupportsButton(bool supportsButton) {
    _supportsButton = supportsButton;
  }

  bool perform(const CommandArgs& args) {
    return _perform(args);
  }
  bool perform() {
    CommandArgs args;
    return _perform(args);
  }

private:
  const std::string _name;
  const std::string _label;
  CommandFn _perform;
  bool _supportsButton;
};

using CommandPtr = std::shared_ptr<Command>;
