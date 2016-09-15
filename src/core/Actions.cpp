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

ActionsController::ActionsController(Context& context)
: _context(context) {
  registerAsActionHandler();
}

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

void ActionsController::update() {
  auto now = _context.time();
  std::vector<Entry> newEntries;
  for (auto i = _actions.begin();
       i != _actions.end();) {
    auto& entry = *i;
    if (now >= entry.time) {
      auto result = (*entry.action)(_context, *this);
      if (result.isReschedule()) {
        newEntries.push_back(entry.withTime(result.rescheduleTime()));
      }
      i = _actions.erase(i);
    } else {
      i++;
    }
  }
  _actions.insert(_actions.end(), newEntries.begin(), newEntries.end());
}

bool ActionsController::performAction(AppAction action) {
  switch (action) {
    case AppAction::TEST_ACTION:
      addDelayed(10, std::make_shared<LoggingAction>("omglol"));
    default:
      return false;
  }
  return true;
}
