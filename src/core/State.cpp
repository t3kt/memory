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
      << ", running: " << running;
}
