//
//  Interval.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include "Interval.h"

Interval::Params::Params()
: ::Params() {
  add(interval
      .setKey("interval")
      .setName("Interval")
      .setParamValuesAndDefaults(2, 6)
      .setParamRanges(0, 20));
}

Interval::Interval(const Interval::Params& params, const State& state)
: _params(params) {
  loadNext(state);
}

void Interval::loadNext(const State& state) {
  _nextTime = state.time + _params.interval.getValue();
}

bool Interval::check(const State &state) {
  if (state.time < _nextTime) {
    return false;
  }
  loadNext(state);
  return true;
}
