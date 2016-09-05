//
//  Spawner.h
//  memory
//
//  Created by tekt on 7/14/16.
//
//

#ifndef Spawner_h
#define Spawner_h

#include <memory>
#include "../core/Context.h"
#include "../core/Params.h"

class Spawner {
public:
  using Params = ParamsWithEnabled;

  virtual void update(Context& context) = 0;

  virtual bool spawnNow(Context& context, int count) = 0;
};

class RateSpawnerParams : public Spawner::Params {
public:
  RateSpawnerParams() {
    add(rate
        .setKey("rate")
        .setName("Rate")
        .setValueAndDefault(4)
        .setRange(0, 30));
  }

  TParam<float> rate;
};

template<typename P = RateSpawnerParams>
class RateSpawner
: public Spawner {
public:
  using Params = P;

  RateSpawner(const P& params)
  : _params(params)
  , _lastTime(-1) { }

  void update(Context& context) override {
    if (!_params.enabled()) {
      _lastTime = -1;
      return;
    }
    float now = context.time();
    if (_lastTime < 0) {
      _lastTime = now;
      return;
    }
    float elapsed = now - _lastTime;
    float count = elapsed * _params.rate();
    if (count > 1) {
      spawnEntities(context, static_cast<int>(count));
      _lastTime = now;
    }
  }

  bool spawnNow(Context& context, int count) override {
    spawnEntities(context, count);
    return true;
  }

protected:
  virtual void spawnEntities(Context& context, int count) = 0;

  const P& _params;
  float _lastTime;
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

#endif /* Spawner_h */
