//
//  Scheduler.h
//
//  Objects used to determine whether some action should performed
//  at the current time.
//

#pragma once

#include <ofMath.h>
#include "../control/Params.h"
#include "../control/ValueSupplier.h"
#include "../core/Common.h"
#include "../core/Context.h"

class SchedulerParams
: public ParamsWithEnabled {
public:
  SchedulerParams() {
    add(chance
        .setKey("chance")
        .setName("Chance")
        .setValueAndDefault(1)
        .setRange(0, 1));
  }

  TParam<float> chance;
};

// Base class for objects used to determine whether some action
// should be performed at the current time.
template<typename P>
class Scheduler
: public NonCopyable {
public:
  using Params = P;

  Scheduler(Context& context,
            const Params& params)
  : _context(context)
  , _params(params) { }

  // Check to see how many times the action should be performed.
  // Returning a count rather than a bool allows some scheduler
  // implementations to be triggered multiple times per update cycle.
  virtual int query() {
    if (!checkEnabled()) {
      return 0;
    }
    if (!checkChance()) {
      return 0;
    }
    return 1;
  }

protected:
  bool checkEnabled() const {
    return _params.enabled.get();
  }
  bool checkChance() const {
    auto chance = _params.chance.get();
    if (chance >= 1) {
      return true;
    }
    auto val = ofRandomuf();
    return val < chance;
  }

  Context& _context;
  const Params& _params;
};

class IntervalSchedulerParams
: public SchedulerParams {
public:
  IntervalSchedulerParams() {
    add(interval
        .setKey("interval")
        .setName("Interval")
        .setParamRanges(0, 60)
        .setParamValuesAndDefaults(4, 10));
  }

  RandomValueSupplier<float> interval;
};

class IntervalScheduler
: public Scheduler<IntervalSchedulerParams> {
public:
  IntervalScheduler(Context& context, const Params& params)
  : Scheduler(context, params)
  , _nextTime(-1) { }

  int query() override;

protected:
  float _nextTime;
};

class RateSchedulerParams
: public SchedulerParams {
public:
  RateSchedulerParams() {
    add(rate
        .setKey("rate")
        .setName("Rate")
        .setValueAndDefault(4)
        .setRange(0, 60));
  }

  TParam<float> rate;
};

class RateScheduler
: public Scheduler<RateSchedulerParams> {
public:
  RateScheduler(Context& context, const Params& params)
  : Scheduler(context, params)
  , _lastTime(-1) { }

  int query() override;

protected:
  float _lastTime;
};
