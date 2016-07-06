//
//  ParticleObject.h
//  memory-prototype-2
//
//  Created by tekt on 6/26/16.
//
//

#ifndef ParticleObject_h
#define ParticleObject_h

#include <vector>
#include "WorldObject.h"
#include "State.h"
#include "Params.h"
#include "Events.h"
#include "Bounds.h"

class ParticleObject : public WorldObject {
public:
  class Params : public ::Params {
  public:
    Params()
    : ::Params() {
      add(_damping
          .setKey("damping")
          .setName("Damping")
          .setValueAndDefault(0.01)
          .setRange(0, 1));
      add(_speed
          .setKey("speed")
          .setName("Speed")
          .setValueAndDefault(1)
          .setRange(0, 10));
    }

    float damping() const { return _damping.get(); }
    float speed() const { return _speed.get(); }

  private:
    TParam<float> _damping;
    TParam<float> _speed;
  };

  ParticleObject(ofVec3f pos, const Params& params, const State& state);

  void setInitialCondition(ofVec3f pos, ofVec3f vel);
  void setVelocity(ofVec3f velocity) { _velocity = velocity; }

  void resetForce();
  void addForce(ofVec3f force);
  void addDampingForce();

  ofVec3f* positionPtr() { return &_position; }
  ofVec3f* velocityPtr() { return &_velocity; }
  ofVec3f* forcePtr() { return &_force; }

  virtual void update(const State& state) override;
protected:
  virtual void outputFields(std::ostream& os) const override;
private:
  const Params& _params;
  ofVec3f _velocity;
  ofVec3f _force;
};

#endif /* ParticleObject_h */
