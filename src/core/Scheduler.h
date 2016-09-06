//
//  Scheduler.h
//  memory
//
//  Created by tekt on 9/5/16.
//
//

#ifndef Scheduler_h
#define Scheduler_h

#include <ofMath.h>
#include "../core/Context.h"
#include "../core/Params.h"
#include "../core/ValueSupplier.h"

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

class Scheduler {
public:
  using Params = SchedulerParams;

  Scheduler(Context& context,
            const Params& params)
  : _context(context)
  , _params(params) { }

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
private:
  const Params& _params;
};

//template<typename P = SchedulerParams>
//class Scheduler
//: public AbstractScheduler {
//public:
//  using Params = typename P;
//
//protected:
//};

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
: public Scheduler {
public:
  using Params = IntervalSchedulerParams;

  IntervalScheduler(Context& context, const Params& params)
  : Scheduler(context, params)
  , _params(params)
  , _nextTime(-1) { }

  int query() override;

protected:
  const Params& _params;
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
: public Scheduler {
public:
  using Params = RateSchedulerParams;

  RateScheduler(Context& context, const Params& params)
  : Scheduler(context, params)
  , _params(params)
  , _lastTime(-1) { }

  int query() override;

protected:
  const Params& _params;
  float _lastTime;
};

#endif /* Scheduler_h */
