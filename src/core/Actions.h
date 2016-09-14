//
//  Actions.h
//  memory
//
//  Created by tekt on 9/13/16.
//
//

#ifndef Actions_h
#define Actions_h

#include <memory>
#include <vector>

class ActionsController;
class Context;

class ActionResult {
public:
  static ActionResult cancel() { return ActionResult(-1); }
  static ActionResult reschedule(float time) {
    return ActionResult(time);
  }

  bool isReschedule() const { return _time < 0; }
  float rescheduleTime() const { return _time; }
private:
  ActionResult(float time) : _time(time) { }

  const float _time;
};

class Action {
public:
  virtual ActionResult run(Context& context,
                           ActionsController& controller) = 0;
};

using ActionPtr = std::shared_ptr<Action>;

class ActionsController {
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

  void addActionAt(ActionPtr action, float time);
  void addDelayed(ActionPtr action, float delay);
  void update();
private:
  Context& _context;
  std::vector<Entry> _actions;
};

#endif /* Actions_h */
