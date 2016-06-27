//
//  ParticleObject.h
//  memory-prototype-2
//
//  Created by tekt on 6/26/16.
//
//

#ifndef ParticleObject_h
#define ParticleObject_h

#include "WorldObject.h"
#include "State.h"

class ParticleObject : public StandardWorldObject {
public:
  ParticleObject(ofVec3f pos);

  void setInitialCondition(ofVec3f pos, ofVec3f vel);
  void setVelocity(ofVec3f velocity) { _velocity = velocity; }
  void setDamping(float damping) { _damping = damping; }

  void resetForce();
  void addForce(ofVec3f force);
  void addDampingForce();

  virtual void update(const State& state) override;
private:
  ofVec3f _velocity;
  ofVec3f _force;
  float _damping;
};

#endif /* ParticleObject_h */
