//
//  State.h
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#ifndef __behavior__State__
#define __behavior__State__

#include "EntityGroup.h"
#include "Attractor.h"

class State {
public:
  State();
  void updateTime();
  
  EntityGroup<Entity> entities;
  EntityGroup<Attractor> attractors;
  float time;
  float timeDelta;
};

#endif /* defined(__behavior__State__) */
