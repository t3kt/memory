//
//  Logging.h
//  memory
//
//  Created by tekt on 7/25/16.
//
//

#ifndef Logging_h
#define Logging_h

#include <functional>
#include <memory>
#include <ofLog.h>
#include <string>
#include "../core/Common.h"
#include "../core/Params.h"

class Logger {
public:
  using Statement = std::function<void(ofLog& log)>;

  class Params : public ParamsWithEnabled {
  public:
    Params() {
      add(level
          .setKey("level")
          .setName("Level")
          .setRange((int) OF_LOG_VERBOSE, (int) OF_LOG_FATAL_ERROR)
          .setValueAndDefault(OF_LOG_NOTICE));
      setEnabledValueAndDefault(true);
    }

    TParam<int> level;
  };

  Logger(const Params& params,
         const bool& allEnabled)
  : _label(params.getKey())
  , _params(params)
  , _allEnabled(allEnabled) { }

  inline bool check(ofLogLevel level) const {
    return _allEnabled && _params.enabled.get() && level >= _params.level.get();
  }

  void log(ofLogLevel level, std::string message) const;

  void log(ofLogLevel level, Statement statement) const;

  void logWarning(std::string message) const {
    log(OF_LOG_WARNING, message);
  }
  void logError(std::string message) const {
    log(OF_LOG_ERROR, message);
  }
  void logNotice(std::string message) const {
    log(OF_LOG_NOTICE, message);
  }
  void logVerbose(std::string message) const {
    log(OF_LOG_VERBOSE, message);
  }

  void logWarning(Statement statement) const {
    log(OF_LOG_WARNING, statement);
  }
  void logError(Statement statement) const {
    log(OF_LOG_ERROR, statement);
  }
  void logNotice(Statement statement) const {
    log(OF_LOG_NOTICE, statement);
  }
  void logVerbose(Statement statement) const {
    log(OF_LOG_VERBOSE, statement);
  }

protected:
  const std::string _label;
  const Params& _params;
  const bool& _allEnabled;
};

class LoggingController {
public:
  class Params : public ParamsWithEnabled {
  public:
    Params() {
      add(app
          .setKey("app")
          .setName("App"));
      add(control
          .setKey("ctrl")
          .setName("Control"));
      add(animation
          .setKey("anim")
          .setName("Animation"));
      add(navigation
          .setKey("nav")
          .setName("Navigation"));
      add(node
          .setKey("node")
          .setName("Node"));
      add(observer
          .setKey("obs")
          .setName("Observer"));
      add(occurrence
          .setKey("occ")
          .setName("Occurrence"));
    }
    Logger::Params app;
    Logger::Params control;
    Logger::Params animation;
    Logger::Params navigation;
    Logger::Params node;
    Logger::Params observer;
    Logger::Params occurrence;
  };

  LoggingController(Params& params)
  : _params(params)
  , _app(params.app, params.enabled.get())
  , _control(params.control, params.enabled.get())
  , _animation(params.animation, params.enabled.get())
  , _navigation(params.navigation, params.enabled.get())
  , _node(params.node, params.enabled.get())
  , _observer(params.observer, params.enabled.get())
  , _occurrence(params.occurrence, params.enabled.get()) { }

  Logger& app() { return _app; }
  Logger& control() { return _control; }
  Logger& animation() { return _animation; }
  Logger& navigation() { return _navigation; }
  Logger& node() { return _node; }
  Logger& observer() { return _observer; }
  Logger& occurrence() { return _occurrence; }

private:
  const Params& _params;
  Logger _app;
  Logger _control;
  Logger _animation;
  Logger _navigation;
  Logger _node;
  Logger _observer;
  Logger _occurrence;
};

#endif /* Logging_h */
