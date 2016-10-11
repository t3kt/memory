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
