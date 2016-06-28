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
: time(ofGetElapsedTimef())
, timeDelta(0) {
  
}

void State::updateTime() {
  auto t = ofGetElapsedTimef();
  timeDelta = t - time;
  time = t;
}

void State::output(std::ostream &os) const {
  os << "State{time: " << time
      << ", timeDelta: " << timeDelta
      << "}";
}
