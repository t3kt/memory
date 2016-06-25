//
//  WorldObject.h
//  memory-prototype-2
//
//  Created by tekt on 6/24/16.
//
//

#ifndef WorldObject_h
#define WorldObject_h

#include "Common.h"
#include "Params.h"
#include <iostream>
#include <ofVec3f.h>

typedef long ObjectId;

class State;

class WorldObject : public Outputable {
public:
  typedef ::Params Params;
  
  WorldObject();
  virtual ~WorldObject() {}
  
  const ObjectId id;
  
  virtual void update(const State& state) {}
  virtual void draw(const State& state) {}
  
  virtual bool isAlive(const State& state) const { return true; };
  
  virtual void handleDeath() {};
  
  virtual void output(std::ostream& os) const override;
  
  ofVec3f position;
};

#endif /* WorldObject_h */
