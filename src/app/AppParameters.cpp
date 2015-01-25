//
//  AppParameters.cpp
//  behavior
//
//  Created by tekt on 1/25/15.
//
//

#include "AppParameters.h"

BehaviorAppParameters::BehaviorAppParameters()
: centerAttraction("Center Attraction")
, entityAttraction("Entity Attraction")
, entityRepulsion("Entity Repulsion")
, attractorWalk("Attractor Walk")
, repulsorWalk("Repulsor Walk")
, otherWalk("Other Walk") {
  paramGroup.add(centerAttraction.paramGroup);
  paramGroup.add(entityAttraction.paramGroup);
  paramGroup.add(attractorWalk.paramGroup);
  paramGroup.add(entityRepulsion.paramGroup);
  paramGroup.add(repulsorWalk.paramGroup);
  paramGroup.add(otherWalk.paramGroup);
}
