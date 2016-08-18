//
//  SimulationStats.cpp
//  memory
//
//  Created by tekt on 8/17/16.
//
//

#include "../analysis/SimulationStats.h"

void SimulationStats::outputFields(std::ostream &os) const {
  os << "observerCount: " << observerCount;
  os << ", occurrenceCount: " << occurrenceCount;
  os << ", navigatorCount: " << navigatorCount;
  os << ", animationCount: " << animationCount;
}
