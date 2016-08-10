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
#include "Bounds.h"
#include "Events.h"
#include "State.h"
#include "WorldObject.h"

class ParticleObject : public WorldObject {
public:
  ParticleObject(ofVec3f pos);

  void setVelocity(ofVec3f velocity) { _velocity = velocity; }

  void resetForce();
  void addForce(ofVec3f force);

  void updateVelocityAndPosition(const State& state, float speed);

  const ofVec3f& position() const override { return _position; }

  const ofVec3f& startPosition() const { return _startPosition; }
  const ofVec3f& velocity() const { return _velocity; }

  ofVec3f* positionPtr() { return &_position; }
  ofVec3f* velocityPtr() { return &_velocity; }
  ofVec3f* forcePtr() { return &_force; }

  virtual void deserializeFields(const Json& obj,
                                 const SerializationContext& context) override;

  virtual void fillInfo(Info& info) const override;

protected:
  ParticleObject() { }

  virtual void outputFields(std::ostream& os) const override;
  virtual void addSerializedFields(Json::object& obj,
                                   const SerializationContext& context) const override;
  ofVec3f _position;
private:
  ofVec3f _velocity;
  ofVec3f _force;
  ofVec3f _startPosition;
};

#endif /* ParticleObject_h */
