//
//  Spawner.h
//

#pragma once

#include <memory>
#include <ofxTCommon.h>
#include "../control/Params.h"
#include "../control/ValueRange.h"
#include "../core/Component.h"
#include "../core/Context.h"
#include "../core/Scheduler.h"

class Spawner
: public ofxTCommon::NonCopyable
, public ComponentBase {
public:
  using Params = ParamsWithEnabled;

  virtual bool spawnNow(int count) = 0;
};

using RateSpawnerParams = RateSchedulerParams;

template<typename P = RateSpawnerParams>
class RateSpawner
: public Spawner {
public:
  using Params = P;

  RateSpawner(Context& context, const P& params)
  : _params(params)
  , _context(context)
  , _scheduler(context, params) { }

  void update() override {
    if (_context.spawningSuspended) {
      return;
    }
    int count = _scheduler.query();
    if (count > 0) {
      spawnEntities(count);
    }
  }

  bool spawnNow(int count) override {
    spawnEntities(count);
    return true;
  }

protected:
  virtual void spawnEntities(int count) = 0;

  const P& _params;
  Context& _context;
  RateScheduler _scheduler;
};

class AbstractDescendantSpawnerParams
: public Spawner::Params {
public:
  AbstractDescendantSpawnerParams() {
    add(threshold
        .setKey("threshold")
        .setName("Spawn Threshold"));
    add(childCountRange
        .setKey("childCountRange")
        .setName("Child Count Range")
        .setParamValuesAndDefaults(0, 3)
        .setParamRanges(0, 15));
  }

  ValueRange<int> childCountRange;
  TParam<float> threshold;
};

class AbstractDescendantSpawner {
public:
  using Params = AbstractDescendantSpawnerParams;

  AbstractDescendantSpawner(const Params& params)
  : _params(params) { }

protected:
  const Params& _params;
};

