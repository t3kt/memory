//
//  State.cpp
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#include "State.h"

#include <ofMain.h>

State::State()
: time(0)
, timeDelta(0) {
  
}

void State::output(std::ostream &os) const {
  os << "State{time: " << time
      << ", timeDelta: " << timeDelta
      << "}";
}
