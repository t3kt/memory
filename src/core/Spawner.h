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
#include "Context.h"
#include "Params.h"

using SpawnerParams = ParamsWithEnabled;

class Spawner {
public:
  using Params = ParamsWithEnabled;

  virtual void update(Context& context) = 0;
};

class IntervalSpawner
: public Spawner {
public:
  class Params : public Spawner::Params {
  public:
    Params() {
      add(interval
          .setKey("interval")
          .setName("Interval")
          .setValueAndDefault(4)
          .setRange(0, 30));
    }

    void setIntervalValueAndDefault(float val) {
      interval.setValueAndDefault(val);
    }

    TParam<float> interval;
  };

  IntervalSpawner(const Params& params)
  : _params(params) {}

  void update(Context& context) override {
    if (!_params.enabled()) {
      return;
    }
    float now = context.time();
    if (_nextTime < 0 || now >= _nextTime) {
      spawnEntities(context);
      _nextTime = now + _params.interval();
    }
  }
protected:
  virtual void spawnEntities(Context& context) = 0;

  const Params& _params;
  float _nextTime;
};

class RateSpawner
: public Spawner {
public:
  class Params : public Spawner::Params {
  public:
    Params() {
      add(_rate
          .setKey("rate")
          .setName("Rate")
          .setValueAndDefault(4)
          .setRange(0, 30));
    }

    Params& setRateValueAndDefault(float rate) {
      _rate.setValueAndDefault(rate);
      return *this;
    }

    Params& setRateRange(float low, float high) {
      _rate.setRange(low, high);
      return *this;
    }

    float rate() const { return _rate.get(); }
  private:
    TParam<float> _rate;
  };

  RateSpawner(const Params& params)
  : _params(params)
  , _lastTime(-1) { }

  void update(Context& context) override {
    if (!_params.enabled()) {
      return;
    }
    float now = context.time();
    if (_lastTime == -1) {
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

protected:
  virtual void spawnEntities(Context& context, int count) = 0;

  const Params& _params;
  float _lastTime;
};


#endif /* Spawner_h */
