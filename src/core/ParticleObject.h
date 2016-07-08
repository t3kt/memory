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
#include "Events.h"
#include "Bounds.h"

class ParticleObject : public WorldObject {
public:
  ParticleObject(ofVec3f pos, const State& state);

  void setVelocity(ofVec3f velocity) { _velocity = velocity; }

  void resetForce();
  void addForce(ofVec3f force);
  void addDampingForce(float damping);

  void updateVelocityAndPosition(const State& state, float speed);

  const ofVec3f& startPosition() const { return _startPosition; }

  ofVec3f* positionPtr() { return &_position; }
  ofVec3f* velocityPtr() { return &_velocity; }
  ofVec3f* forcePtr() { return &_force; }

protected:
  virtual void outputFields(std::ostream& os) const override;
private:
  ofVec3f _velocity;
  ofVec3f _force;
  ofVec3f _startPosition;
};

#endif /* ParticleObject_h */
