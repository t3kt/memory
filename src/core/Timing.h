//
//  Timing.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef Timing_h
#define Timing_h

#include <list>
#include <memory>

class TimedAction {
public:
  virtual ~TimedAction() {}
  virtual bool done() const = 0;
  virtual bool update(float args) = 0;
};

class DurationAction : public TimedAction {
public:
  DurationAction(float start, float end)
  : _startTime(start), _endTime(end), _started(false), _ended(false) { }
  virtual ~DurationAction() override {}
  
  bool started() const { return _started; }
  virtual bool done() const override { return _ended; }
  
  virtual bool update(float time) override;
  
protected:
  virtual void call(float time, float percentage) = 0;
  virtual void start() {
    _started = true;
  }
  virtual void end() {
    _ended = true;
  }
  
private:
  float _startTime;
  float _endTime;
  bool _started;
  bool _ended;
};

class TimedActionSet : public TimedAction {
public:
  TimedActionSet(bool autoRemove = true)
  : _autoRemove(autoRemove) { }
  
  virtual ~TimedActionSet() {}
  
  template<typename ...Args>
  void add(std::shared_ptr<TimedAction> action, Args&... moreActions) {
    add(action);
    add(moreActions...);
  }
  
  void add(std::shared_ptr<TimedAction> action) {
    _actions.push_back(action);
  }
  
  virtual bool done() const override;
  
  virtual bool update(float time) override;
  
  int size() const { return _actions.size(); }
  
  void clear();
private:
  bool _autoRemove;
  std::list<std::shared_ptr<TimedAction> > _actions;
};

#endif /* Timing_h */
