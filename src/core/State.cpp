//
//  State.cpp
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#include "../core/State.h"

void ClockState::outputFields(std::ostream &os) const {
  os << "localTime: " << localTime
      << ", timeDelta: " << timeDelta
      << ", running: " << running
      << ", rate: " << rate;
}

void AgeTracker::outputFields(std::ostream &os) const {
  os << "age: " << get();
//  os << ", startTime: " << _startTime;
}

void AgeTracker::setAge(float age) {
  _startTime = _state.localTime - age;
}
