//
//  Behavior.h
//  behavior
//
//  Created by tekt on 1/21/15.
//
//

#ifndef __behavior__Behavior__
#define __behavior__Behavior__

#include <ofTypes.h>
#include <ofParameterGroup.h>
#include <vector>

class Entity;
class State;

class Behavior {
public:
  class Params {
  public:
    Params();
    
    ofParameter<bool> enabled;
    ofParameterGroup paramGroup;
  };
  
  virtual void update(Entity& entity,
                      State& state) = 0;
};

typedef std::vector<shared_ptr<Behavior> > BehaviorList;

void updateBehaviors(BehaviorList& behaviors,
                     Entity& entity,
                     State& state);

#endif /* defined(__behavior__Behavior__) */
