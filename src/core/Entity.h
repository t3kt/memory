//
//  Entity.h
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#ifndef __behavior__Entity__
#define __behavior__Entity__

#include <ofVec3f.h>
#include <ofColor.h>

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
};

#endif /* defined(__behavior__Entity__) */
