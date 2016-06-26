//
//  Interval.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include "Interval.h"

Interval::Params::Params(std::string name)
: ::Params(name)
, interval("Interval") {
  add(interval.set(2, 6)
      .setParamRange(0, 20));
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
