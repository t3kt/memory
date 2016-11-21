//
//  ObserverSickness.h
//

#pragma once

#include "../control/Params.h"
#include "../core/Common.h"
#include "../core/Component.h"
#include "../core/Context.h"
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
: public ofxTCommon::NonCopyable
, public ComponentBase {
public:
  using Params = ObserverSicknessParams;

  ObserverSickness(Context& context, const Params& params)
  : _context(context)
  , _params(params)
  , _scheduler(context, params.scheduler) { }

  void update() override;

private:
  Context& _context;
  const Params& _params;
  IntervalScheduler _scheduler;
};

