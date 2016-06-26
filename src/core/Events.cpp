//
//  Events.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include "Events.h"

void StateEventArgs::output(std::ostream &os) const {
  os << "StateEventArgs{"
    << state
    << "}";
}
