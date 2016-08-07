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

class Spawner {
public:
  using Params = ParamsWithEnabled;

  virtual void update(Context& context) = 0;
};

class IntervalSpawnerParams : public Spawner::Params {
public:
  IntervalSpawnerParams() {
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

template<typename P = IntervalSpawnerParams>
class IntervalSpawner
: public Spawner {
public:
  using Params = P;

  IntervalSpawner(const P& params)
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

  const P& _params;
  float _nextTime;
};

class RateSpawnerParams : public Spawner::Params {
public:
  RateSpawnerParams() {
    add(_rate
        .setKey("rate")
        .setName("Rate")
        .setValueAndDefault(4)
        .setRange(0, 30));
  }

  RateSpawnerParams& setRateValueAndDefault(float rate) {
    _rate.setValueAndDefault(rate);
    return *this;
  }

  RateSpawnerParams& setRateRange(float low, float high) {
    _rate.setRange(low, high);
    return *this;
  }

  float rate() const { return _rate.get(); }
private:
  TParam<float> _rate;
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

  const P& _params;
  float _lastTime;
};

class AbstractDescendantSpawner {
public:
  class Params : public Spawner::Params {
  public:
    Params() {
      add(_threshold
          .setKey("threshold")
          .setName("Spawn Threshold"));
      add(childCountRange
          .setKey("childCountRange")
          .setName("Child Count Range")
          .setParamValuesAndDefaults(0, 3)
          .setParamRanges(0, 15));
    }

    float threshold() const { return _threshold.get(); }

    ValueRange<int> childCountRange;

  private:
    TParam<float> _threshold;
  };

  AbstractDescendantSpawner(const Params& params)
  : _params(params) { }

protected:
  const Params& _params;
};

#endif /* Spawner_h */
