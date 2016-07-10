//
//  State.cpp
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#include "State.h"

State::State()
: time(0)
, timeDelta(0) { }

void State::output(std::ostream &os) const {
  os << "State{time: " << time
      << ", timeDelta: " << timeDelta
      << "}";
}

void ChangeFlag::output(std::ostream &os) const {
  os << _name << ":" << std::boolalpha << _status;
}

void ChangeFlags::output(std::ostream& os) const {
  os << "ChangeFlags{"
  << boundsChanged
  << "}";
}

void ChangeFlags::clear() {
  boundsChanged.reset();
}
