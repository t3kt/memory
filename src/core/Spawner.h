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
#include "Params.h"
#include "State.h"

using SpawnerParams = ParamsWithEnabled;

class IntervalSpawner {
public:
  class Params : public SpawnerParams {
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

  void update(const State& state) {
    if (!_params.enabled()) {
      return;
    }
    if (_nextTime < 0 || state.time >= _nextTime) {
      spawnEntities(state);
      _nextTime = state.time + _params.interval();
    }
  }
protected:
  virtual void spawnEntities(const State& state) = 0;

  const Params& _params;
  float _nextTime;
};

class RateSpawner {
public:
  class Params : public SpawnerParams {
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

  void update(const State& state) {
    if (!_params.enabled()) {
      return;
    }
    if (_lastTime == -1) {
      _lastTime = state.time;
      return;
    }
    float elapsed = state.time - _lastTime;
    float count = elapsed * _params.rate();
    if (count > 1) {
      spawnEntities(state, static_cast<int>(count));
      _lastTime = state.time;
    }
  }

protected:
  virtual void spawnEntities(const State& state, int count) = 0;

  const Params& _params;
  float _lastTime;
};


#endif /* Spawner_h */
