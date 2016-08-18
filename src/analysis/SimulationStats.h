//
//  SimulationStats.h
//  memory
//
//  Created by tekt on 8/17/16.
//
//

#ifndef SimulationStats_h
#define SimulationStats_h

#include <cstddef>
#include "../core/Common.h"

class EntityStats
: public Outputable {
public:
  using CountT = std::size_t;

  CountT living;
  CountT spawned;
  CountT died;
  CountT totalSpawned;
  CountT totalDied;

  void resetCurrentCounts() {
    living = 0;
    spawned = 0;
    died = 0;
  }

  std::string typeName() const override { return "EntityStats"; }
protected:
  void outputFields(std::ostream& os) const override;
};

class SimulationStats
: public Outputable {
public:
  using CountT = std::size_t;

  EntityStats animations;
  EntityStats navigators;
  EntityStats observers;
  EntityStats occurrences;

  void resetCurrentCounts() {
    animations.resetCurrentCounts();
    navigators.resetCurrentCounts();
    observers.resetCurrentCounts();
    occurrences.resetCurrentCounts();
  }

  std::string typeName() const override { return "SimulationStats"; }
protected:
  void outputFields(std::ostream& os) const override;
};

#endif /* SimulationStats_h */
