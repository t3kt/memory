//
//  Clock.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#include "Clock.h"

Clock::Params::Params()
: ::Params("Clock") {
  add(paused.set("Paused", false));
  add(rate.set("Rate", 1, 0, 4));
}

Clock::Clock(const Clock::Params& params, State& state)
: _params(params)
, _state(state) {
}
