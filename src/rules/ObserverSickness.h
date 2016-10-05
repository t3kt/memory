//
//  ObserverSickness.h
//  memory
//
//  Created by tekt on 9/5/16.
//
//

#ifndef ObserverSickness_h
#define ObserverSickness_h

#include "../core/Common.h"
#include "../core/Context.h"
#include "../core/Params.h"
#include "../core/Scheduler.h"

class ObserverSicknessParams
: public ParamsWithEnabled {
public:
  ObserverSicknessParams() {
    add(scheduler
        .setKey("scheduler")
        .setName("Scheduler"));
    add(decayRateAmount
        .setKey("decayRateAmount")
        .setName("Decay Rate Amount")
        .setRange(-0.2, 0.2)
        .setValueAndDefault(0.06));
  }

  IntervalSchedulerParams scheduler;
  TParam<float> decayRateAmount;
};

class ObserverSickness
: public NonCopyable {
public:
  using Params = ObserverSicknessParams;

  ObserverSickness(Context& context, const Params& params)
  : _context(context)
  , _params(params)
  , _scheduler(context, params.scheduler) { }

  void update();

private:
  Context& _context;
  const Params& _params;
  IntervalScheduler _scheduler;
};

#endif /* ObserverSickness_h */
