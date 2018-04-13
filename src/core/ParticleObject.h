//
//  ParticleObject.h
//  memory-prototype-2
//
//  Created by tekt on 6/26/16.
//
//

#pragma once

#include <memory>
#include <vector>
#include "../core/State.h"
#include "../core/WorldObject.h"

// Base class for objects which have particle-based position and
// motion behavior.
// In each frame, each particle object has a position and a velocity.
// Some number of forces are applied to it, which modify the velocity,
// which in turn modifies the position.
class ParticleObject
: public WorldObject
, public std::enable_shared_from_this<ParticleObject> {
public:
  ParticleObject(ofVec3f pos,
                 const ClockState& state);

  bool isParticle() const override { return true; }

  void setVelocity(ofVec3f velocity) { _velocity = velocity; }

  void resetForce();
  void addForce(ofVec3f force);

  void updateVelocityAndPosition(const ClockState& state,
                                 float speed);

  const ofVec3f& position() const override { return _position; }

  const ofVec3f& startPosition() const { return _startPosition; }
  const ofVec3f& velocity() const { return _velocity; }

  ofVec3f* positionPtr() { return &_position; }
  ofVec3f* velocityPtr() { return &_velocity; }
  ofVec3f* forcePtr() { return &_force; }

  float size() const { return _size; }
  void setSize(float size) { _size = size; }

  virtual void deserializeFields(const ofJson& obj,
                                 const SerializationContext& context) override;

  virtual void fillInfo(Info& info) const override;

protected:
  ParticleObject(const ClockState& state)
  : WorldObject(state) { }

  virtual void outputFields(std::ostream& os) const override;
  virtual void addSerializedFields(ofJson& obj,
                                   const SerializationContext& context) const override;
  ofVec3f _position;
private:
  ofVec3f _velocity;
  ofVec3f _force;
  ofVec3f _startPosition;
  float _size;
};

using ParticlePtr = std::shared_ptr<ParticleObject>;
using ParticleVector = std::vector<ParticlePtr>;

