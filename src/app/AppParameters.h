//
//  AppParameters.h
//  behavior
//
//  Created by tekt on 1/25/15.
//
//

#ifndef __behavior__AppParameters__
#define __behavior__AppParameters__

#include <ofParameterGroup.h>
#include "AttractionBehavior.h"
#include "RandomWalkBehavior.h"

#include "ObserverEntity.h"

class BehaviorAppParameters {
public:
  BehaviorAppParameters();
  ofParameterGroup paramGroup;
  SingleAttractionBehavior::Params centerAttraction;
  EntityAttractionBehavior::Params entityAttraction;
  EntityAttractionBehavior::Params entityRepulsion;
  RandomWalkBehavior::Params attractorWalk;
  RandomWalkBehavior::Params repulsorWalk;
  RandomWalkBehavior::Params otherWalk;
};

class MemoryAppParameters {
public:
  MemoryAppParameters() {}
  
  ObserverEntity::Params observer;
};

#endif /* defined(__behavior__AppParameters__) */
