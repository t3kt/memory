//
//  Command.h
//  memory
//

#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <Poco/Any.h>
#include <string>
#include <unordered_map>
#include <vector>

class Context;

using CommandArg = Poco::Any;

class CommandArgs {
public:
  using Arg = CommandArg;
  using Storage = std::vector<Arg>;

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

using CommandFn = std::function<bool(Context&, const CommandArgs&)>;
class Command;
using CommandPtr = std::shared_ptr<Command>;

class Command {
public:
  virtual bool perform(Context& context,
                       const CommandArgs& args) = 0;

  static CommandPtr of(CommandFn fn);
};


class TestCommand
: public Command {
public:
  bool perform(Context& context,
               const CommandArgs& args) override;
};

class CommandSet {
public:
  using CommandMap = std::unordered_map<std::string, CommandPtr>;

  bool perform(const std::string& name, Context& context, const CommandArgs& args) const;

private:
  CommandMap _commands;
};
