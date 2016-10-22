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
#include "../core/Component.h"
#include "../core/Logging.h"

class ActionsController;
class Context;

class ActionResult {
private:
  enum class ResultType {
    RESCHEDULE,
    CONTINUOUS,
    CANCEL,
    ABORT,
  };
public:
  static ActionResult cancel() {
    return ActionResult(-1, ResultType::CANCEL);
  }
  static ActionResult reschedule(float time) {
    return ActionResult(time, ResultType::RESCHEDULE);
  }
  static ActionResult continuous() {
    return ActionResult(-1, ResultType::CONTINUOUS);
  }
  static ActionResult abort() {
    return ActionResult(-1, ResultType::ABORT);
  }

  bool isReschedule() const { return _type == ResultType::RESCHEDULE; }
  float rescheduleTime() const { return _time; }
  bool isContinuous() const { return _type == ResultType::CONTINUOUS; }
  bool isAbort() const { return _type == ResultType::ABORT; }
private:
  ActionResult(float time, ResultType type)
  : _time(time)
  , _type(type) { }

  const float _time;
  const ResultType _type;
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

class AbortableAction
: public Action {
public:
  void abort() { _aborted = true; }
protected:
  bool _aborted;
};

using ActionFinishCallback = std::function<void()>;

class ActionsController
: public AppActionHandler
, public NonCopyable
, public ComponentBase {
private:
  class Entry {
  public:
    Entry(ActionPtr a, float t) : action(a), time(t) { }
    Entry(ActionPtr a, float t, ActionFinishCallback f)
    : action(a)
    , time(t)
    , onFinish(f) { }

    ActionPtr action;
    float time;
    ActionFinishCallback onFinish;

    Entry withTime(float t) { return Entry(action, t, onFinish); }
    Entry asContinuous() { return Entry(action, -1, onFinish); }
  };
public:
  ActionsController(Context& context)
  : _context(context) { }

  void addAt(float time,
             ActionPtr action,
             ActionFinishCallback onFinish=nullptr);
  void addAt(float time,
             ActionFn action,
             ActionFinishCallback onFinish=nullptr);
  void addDelayed(float delay,
                  ActionPtr action,
                  ActionFinishCallback onFinish=nullptr);
  void addDelayed(float delay,
                  ActionFn action,
                  ActionFinishCallback onFinish=nullptr);
  void addRepeating(float interval,
                    std::function<bool()> action,
                    ActionFinishCallback onFinish=nullptr);
  void addContinuous(ActionPtr action,
                     ActionFinishCallback onFinish=nullptr);
  void addContinuous(ActionFn action,
                     ActionFinishCallback onFinish=nullptr);
  void addContinuous(float duration,
                     std::function<bool()> action,
                     ActionFinishCallback onFinish=nullptr);

  void update() override;

  void logAction(std::string message);
  void logAction(Logger::Statement statement);

  bool performAction(AppAction action) override;
private:
  Context& _context;
  std::vector<Entry> _actions;
  std::vector<Entry> _continuousActions;
};

#endif /* Actions_h */
