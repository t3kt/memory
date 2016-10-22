//
//  State.h
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#ifndef __behavior__State__
#define __behavior__State__

#include "../core/Common.h"

class ClockState
: public NonCopyable
, public Outputable {
public:
  ClockState()
  : localTime(0)
  , timeDelta(0)
  , running(true) { }

  float localTime;
  float timeDelta;
  bool running;
  float rate;

  std::string typeName() const override { return "ClockState"; }

protected:
  void outputFields(std::ostream& os) const override;
};

class AgeTracker
: public NonCopyable
, public Outputable {
public:
  AgeTracker(const ClockState& state)
  : _state(state)
  , _startTime(state.localTime) { }

  float get() const {
    return _state.localTime - _startTime;
  }

  operator float() const { return get(); }

  float startTime() const { return _startTime; }

  void setAge(float age);

  std::string typeName() const override { return "Age"; }

protected:
  void outputFields(std::ostream& os) const override;

private:
  const ClockState& _state;
  float _startTime;
};

#endif /* defined(__behavior__State__) */
