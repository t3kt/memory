//
//  Timing.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef Timing_h
#define Timing_h

#include <functional>
#include <list>
#include <memory>

typedef std::function<void(float)> TimedFunction;
typedef std::function<void(float, float)> TimedPercentageFunction;
template<typename T>
using ApplyCallback = std::function<void(const T&)>;

class TimedAction {
public:
  virtual ~TimedAction() {}
  virtual bool done() const = 0;
  virtual bool update(float args) = 0;
};

class OnceAction : public TimedAction {
public:
  static std::shared_ptr<OnceAction>
  newOnceAction(float triggerTime, TimedFunction fn);
  
  OnceAction(float triggerTime)
  : _triggerTime(triggerTime), _called(false) { }
  
  virtual ~OnceAction() override {}
  
  virtual bool done() const override { return _called; }
  
  virtual void call(float time) = 0;
  
  virtual bool update(float time) override;
protected:
  bool _called;
  float _triggerTime;
};

class DurationAction : public TimedAction {
public:
  static std::shared_ptr<DurationAction>
  newDurationAction(float start, float end, TimedPercentageFunction fn);
  
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

template<typename T>
class ValueRampAction : public DurationAction {
public:
  ValueRampAction(float start, float end,
                  const T& startVal, const T& endVal)
  : DurationAction(start, end)
  , _startVal(startVal), _endVal(endVal) { }
  
  virtual void call(float time, float percentage) override {
    T value = getInterpolated(_startVal, _endVal, percentage);
    applyValue(value);
  }
protected:
  virtual void applyValue(const T& value) = 0;
private:
  const T& _startVal;
  const T& _endVal;
};

template<typename T>
class CallbackValueRampAction : public ValueRampAction<T> {
public:
  CallbackValueRampAction(float start, float end,
                          const T& startVal, const T& endVal,
                          ApplyCallback<T> callback)
  : ValueRampAction<T>(start, end, startVal, endVal)
  , _callback(callback) {}
  
protected:
  void applyValue(const T& value) override {
    _callback(value);
  }
  
private:
  const ApplyCallback<T> _callback;
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
