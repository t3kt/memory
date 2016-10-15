//
//  Actions.cpp
//  memory
//
//  Created by tekt on 9/13/16.
//
//

#include "../app/AppSystem.h"
#include "../core/Actions.h"
#include "../core/Context.h"
#include "../core/State.h"

class FnAction : public Action {
public:
  FnAction(ActionFn action) : _action(action) { }

  ActionResult operator()(Context& context,
                          ActionsController& controller) override {
    return _action(context, controller);
  }
private:
  ActionFn _action;
};

ActionPtr Action::of(ActionFn action) {
  return std::make_shared<FnAction>(action);
}

class LoggingAction
: public Action {
public:
  LoggingAction(std::string message) : _message(message) { }
  ActionResult operator()(Context& context,
                          ActionsController& controller) override {
    controller.logAction(_message);
    return ActionResult::cancel();
  }
private:
  const std::string _message;
};

class RepeatingAction
: public Action {
public:
  RepeatingAction(float interval, std::function<bool()> action)
  : _interval(interval)
  , _action(action) { }

  ActionResult operator()(Context& context,
                          ActionsController& controller) override {
    if (_action()) {
      return ActionResult::reschedule(context.time() + _interval);
    } else {
      return ActionResult::cancel();
    }
  }
private:
  float _interval;
  std::function<bool()> _action;
};

class ContinuousDurationFnAction
: public Action {
public:
  ContinuousDurationFnAction(float duration,
                             std::function<bool()> action,
                             const ClockState& clockState)
  : _duration(duration)
  , _action(action)
  , _age(clockState) { }

  ActionResult operator()(Context& context,
                          ActionsController& controller) override {
    if (_age.get() < _duration && _action()) {
      return ActionResult::continuous();
    } else {
      return ActionResult::cancel();
    }
  }

private:
  AgeTracker _age;
  float _duration;
  std::function<bool()> _action;
};

void ActionsController::logAction(std::string message) {
  AppSystem::get().log().app().logNotice([&](ofLog& log) {
    log << "Action at [" << _context.time() << "]: " << message;
  });
}

void ActionsController::logAction(Logger::Statement statement) {
  AppSystem::get().log().app().logNotice([&](ofLog& log) {
    log << "Action at [" << _context.time() << "]: ";
    statement(log);
  });
}

void ActionsController::addAt(float time, ActionPtr action) {
  _actions.push_back(Entry(action, time));
}

void ActionsController::addAt(float time, ActionFn action) {
  addAt(time, Action::of(action));
}

void ActionsController::addDelayed(float delay, ActionPtr action) {
  addAt(_context.time() + delay, action);
}

void ActionsController::addDelayed(float delay, ActionFn action) {
  addDelayed(delay, Action::of(action));
}

void ActionsController::addRepeating(float interval,
                                     std::function<bool ()> action) {
  _actions
  .emplace_back(std::make_shared<RepeatingAction>(interval, action),
                interval);
}

void ActionsController::addContinuous(ActionPtr action) {
  _continuousActions.push_back(action);
}

void ActionsController::addContinuous(ActionFn action) {
  addContinuous(Action::of(action));
}

void ActionsController::addContinuous(float duration,
                                      std::function<bool ()> action) {
  auto contAction =
  std::make_shared<ContinuousDurationFnAction>(duration,
                                               action,
                                               _context.rootState);
  addContinuous(contAction);
}

void ActionsController::update() {
  auto now = _context.time();
  std::vector<Entry> newEntries;
  std::vector<ActionPtr> newContinuousActions;
  if (_context.rootState.running) {
    for (auto i = _continuousActions.begin();
         i != _continuousActions.end();) {
      auto action = *i;
      auto result = (*action)(_context, *this);
      if (result.isContinuous()) {
        i++;
      } else {
        if (result.isReschedule()) {
          newEntries.push_back(Entry(action, result.rescheduleTime()));
        }
        i = _continuousActions.erase(i);
      }
    }
  }
  for (auto i = _actions.begin();
       i != _actions.end();) {
    auto& entry = *i;
    if (now >= entry.time) {
      auto result = (*entry.action)(_context, *this);
      if (result.isContinuous()) {
        newContinuousActions.push_back(entry.action);
      } else if (result.isReschedule()) {
        newEntries.push_back(entry.withTime(result.rescheduleTime()));
      }
      i = _actions.erase(i);
    } else {
      i++;
    }
  }
  _actions.insert(_actions.end(),
                  newEntries.begin(),
                  newEntries.end());
  _continuousActions.insert(_continuousActions.end(),
                            newContinuousActions.begin(),
                            newContinuousActions.end());
}

bool ActionsController::performAction(AppAction action) {
  switch (action) {
    case AppAction::TEST_ACTION:
      addDelayed(10, std::make_shared<LoggingAction>("omglol"));
      return true;
    case AppAction::TEST_ACTION_2:
      addContinuous(1, [&]() {
        logAction("fooo");
        return true;
      });
      return true;
    default:
      return false;
  }
}
