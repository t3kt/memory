//
//  Actions.h
//  memory
//
//  Created by tekt on 9/13/16.
//
//

#ifndef Actions_h
#define Actions_h

#include <functional>
#include <memory>
#include <vector>
#include "../app/AppActions.h"
#include "../core/Common.h"
#include "../core/Logging.h"

class ActionsController;
class Context;

class ActionResult {
public:
  static ActionResult cancel() { return ActionResult(-1); }
  static ActionResult reschedule(float time) {
    return ActionResult(time);
  }

  bool isReschedule() const { return _time >= 0; }
  float rescheduleTime() const { return _time; }
private:
  ActionResult(float time) : _time(time) { }

  const float _time;
};

class Action;
using ActionPtr = std::shared_ptr<Action>;

using ActionFn =
std::function<ActionResult(Context&,
                           ActionsController&)>;

class Action {
public:
  virtual ActionResult operator()(Context& context,
                                  ActionsController& controller) = 0;

  static ActionPtr of(ActionFn action);
};

class ActionsController
: public AppActionHandler
, public NonCopyable {
private:
  class Entry {
  public:
    Entry(ActionPtr a, float t) : action(a), time(t) { }
    ActionPtr action;
    float time;

    Entry withTime(float t) { return Entry(action, t); }
  };
public:
  ActionsController(Context& context)
  : _context(context) { }

  void addAt(float time, ActionPtr action);
  void addAt(float time, ActionFn action);
  void addDelayed(float delay, ActionPtr action);
  void addDelayed(float delay, ActionFn action);
  void addRepeating(float interval, std::function<bool()> action);

  void update();

  void logAction(std::string message);
  void logAction(Logger::Statement statement);

  bool performAction(AppAction action) override;
private:
  Context& _context;
  std::vector<Entry> _actions;
};

#endif /* Actions_h */
