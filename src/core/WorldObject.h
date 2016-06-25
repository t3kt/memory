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
  
  virtual void handleDeath() {};
  
  virtual void output(std::ostream& os) const override;
  
  bool alive() const { return _alive; };
  void kill() {
    _alive = false;
    handleDeath();
  }
  void revive() { _alive = true; }
  
  ofVec3f position;
  
private:
  bool _alive;
};

#endif /* WorldObject_h */
