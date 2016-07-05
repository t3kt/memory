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
#include "State.h"
#include <iostream>
#include <ofVec3f.h>

typedef long ObjectId;

class State;

class WorldObject : public Outputable {
public:
  typedef ::Params Params;
  
  WorldObject(const State& state);
  virtual ~WorldObject() {}
  
  const ObjectId id;
  
  virtual void update(const State& state) {}
  virtual void draw(const State& state) {}
  
  virtual void output(std::ostream& os) const override;

  virtual bool alive() const { return _alive; }

  void kill() {
    _alive = false;
    handleDeath();
  }
  void revive() { _alive = true; }
  
  virtual bool visible() const { return this->alive(); }

  ofVec3f position() const { return _position; }

  virtual void handleDeath() {}

  float getAge(const State& state) const { return state.time - _startTime; }

protected:
  virtual std::string typeName() const;
  virtual void outputFields(std::ostream& os) const;

  ofVec3f _position;
  const float _startTime;
private:
  bool _alive;
};

#endif /* WorldObject_h */
