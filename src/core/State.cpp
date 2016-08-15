//
//  State.cpp
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#include <iostream>
#include <string>
#include "../core/State.h"

void State::outputFields(std::ostream &os) const {
  os << "time: " << time
      << ", timeDelta: " << timeDelta
      << ", running: " << running
      << ", observerCount: " << observerCount
      << ", occurrenceCount: " << occurrenceCount
      << ", animationCount: " << animationCount;
}

void ChangeFlag::outputFields(std::ostream &os) const {
  os << _name << "=" << std::boolalpha << _status;
}

void ChangeFlags::outputFields(std::ostream& os) const {
  os << boundsChanged;
}

void ChangeFlags::clear() {
  boundsChanged.reset();
}
