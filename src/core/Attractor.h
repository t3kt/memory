//
//  Attractor.h
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#ifndef __behavior__Attractor__
#define __behavior__Attractor__

#include "Entity.h"

class Attractor : public Entity {
public:
  void draw(State& state) override;
  
  void applyTo(Entity* entity) const;
  
  float range;
  float pull;
};

#endif /* defined(__behavior__Attractor__) */
