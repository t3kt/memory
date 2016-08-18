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

class SimulationStats
: public Outputable {
public:
  using CountT = std::size_t;

  CountT observerCount;
  CountT occurrenceCount;
  CountT navigatorCount;
  CountT animationCount;

  std::string typeName() const override { return "stats"; }
protected:
  void outputFields(std::ostream& os) const override;
};

#endif /* SimulationStats_h */
