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

  template<typename T>
  T getOrDefault(std::size_t i, T defaultValue = T()) const {
    if (hasArgType<T>(i)) {
      return get<T>(i);
    }
    return defaultValue;
  }
  template<typename T>
  bool hasArgType(std::size_t i) const {
    return i < size() && _args[i].type() == typeid(T);
  }

  template<typename T>
  bool hasArg(std::size_t i, const T& value) const {
    return hasArgType<T>(i) && Poco::AnyCast<T>(_args[i]) == value;
  }

private:
  Storage _args;
};

using CommandFn = std::function<bool(const CommandArgs&)>;

class Command;
using CommandPtr = std::shared_ptr<Command>;

class Command
: public ofxTCommon::NonCopyable {
public:
  static CommandPtr of(std::string name,
                       std::string label,
                       CommandFn performFunc);

  Command(std::string name,
          std::string label)
  : _name(name)
  , _label(label)
  , _supportsButton(false) { }

  const std::string& name() const { return _name; }
  const std::string& label() const { return _label; }
  bool supportsButton() const { return _supportsButton; }

  void setSupportsButton(bool supportsButton) {
    _supportsButton = supportsButton;
  }

  virtual bool perform(const CommandArgs& args) = 0;
  bool perform() {
    CommandArgs args;
    return perform(args);
  }

private:
  const std::string _name;
  const std::string _label;
  bool _supportsButton;
};
