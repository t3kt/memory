//
//  Entity.h
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#ifndef __behavior__Entity__
#define __behavior__Entity__

#include <ofTypes.h>
#include <vector>
#include "Behavior.h"

typedef long EntityId;

class State;

class Entity {
public:
  Entity();
  virtual ~Entity() {}
  
  virtual void update(State& state);
  virtual void draw(State& state);
  
  const EntityId id;
  ofVec3f position;
  ofVec3f velocity;
  ofFloatColor color;
  float drawRadius;
  
  Entity& addBehavior(shared_ptr<Behavior> behavior);
  
  template<typename TBehavior>
  TBehavior& addBehavior() {
    shared_ptr<TBehavior> behavior(new TBehavior());
    _behaviors.push_back(behavior);
    return *behavior;
  }
private:
  BehaviorList _behaviors;
};

#endif /* defined(__behavior__Entity__) */
