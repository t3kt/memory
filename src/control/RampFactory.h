//
//  RampFactory.h
//
//

#pragma once

#include <ofxChoreograph.h>
#include "../control/Params.h"

class ClockState;

template<typename T>
class RampFactoryParams
: public Params {
public:
  RampFactoryParams() {
    add(_duration
        .setKey("duration")
        .setName("Duration")
        .setRange(0, 4)
        .setValueAndDefault(1));
    add(_startValue
        .setKey("startValue")
        .setName("Start Value")
        .setRange(0, 1)
        .setValueAndDefault(0));
    add(_endValue
        .setKey("endValue")
        .setName("End Value")
        .setRange(0, 1)
        .setValueAndDefault(1));
  }

  float duration() const { return _duration.get(); }
  const T& startValue() const { return _startValue.get(); }
  const T& endValue() const { return _endValue.get(); }
private:
  TParam<float> _duration;
  TParam<T> _startValue;
  TParam<T> _endValue;
};

template<typename T>
class RampFactory {
public:
  using Params = RampFactoryParams<T>;

  RampFactory(const Params& params)
  : _params(params)
  , _lastParams(params) { }

  void update(const ClockState& state) {
    ParamVals newParams(_params);
    if (newParams != _lastParams) {
      _phrase.reset();
      _lastParams = newParams;
    }
  }

  ofxChoreograph::PhraseRef<T> getPhrase() {
    if (!_phrase) {
      rebuildPhrase();
    }
    return _phrase;
  }

private:

  class ParamVals {
  public:
    ParamVals() {}
    explicit ParamVals(const Params& params)
    : duration(params.duration())
    , startValue(params.startValue())
    , endValue(params.endValue()) { }

    float duration;
    T startValue;
    T endValue;

    friend bool operator==(const ParamVals& rhs, const ParamVals& lhs) {
      return rhs.duration == lhs.duration && rhs.startValue == lhs.startValue && rhs.endValue == lhs.endValue;
    }

    friend bool operator!=(const ParamVals& rhs, const ParamVals& lhs) {
      return !(rhs == lhs);
    }
  };

  void rebuildPhrase() {
    _phrase.reset(new ofxChoreograph::RampTo<T>(_lastParams.duration,
                                                _lastParams.startValue,
                                                _lastParams.endValue));
  }

  const Params& _params;
  ParamVals _lastParams;
  ofxChoreograph::PhraseRef<T> _phrase;
};
