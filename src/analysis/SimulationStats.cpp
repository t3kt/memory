//
//  SimulationStats.cpp
//  memory
//
//  Created by tekt on 8/17/16.
//
//

#include "../analysis/SimulationStats.h"

void EntityStats::outputFields(std::ostream &os) const {
  os << "living: " << living;
  os << ", died: " << died << " (" << totalDied << ")";
  os << ", spawned: " << spawned << " (" << totalSpawned << ")";
}

void SimulationStats::outputFields(std::ostream &os) const {
  os << "observers: " << observers;
  os << ", occurrences: " << occurrences;
  os << ", navigators: " << navigators;
  os << ", animations: " << animations;
}
