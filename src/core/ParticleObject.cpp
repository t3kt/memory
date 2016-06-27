//
//  ParticleObject.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/26/16.
//
//

#include "ParticleObject.h"

ParticleObject::Params::Params(std::string name)
: ::Params(name) {
  add(damping.set("Damping", 0.01, 0, 0.2));
}

ParticleObject::ParticleObject(ofVec3f pos, const ParticleObject::Params& params)
: StandardWorldObject()
, _velocity(0)
, _force(0)
, _params(params) {
  _position = pos;
}

AbstractReboundBehavior::Params::Params(std::string name)
: ::ValueRange<ofVec3f>(name) {
}

void ParticleObject::setInitialCondition(ofVec3f pos, ofVec3f vel) {
  _position = pos;
  _velocity = vel;
}

void ParticleObject::resetForce() {
  _force.set(0);
}

void ParticleObject::addForce(ofVec3f force) {
  _force += force;
}

void ParticleObject::addDampingForce() {
  addForce(_velocity * -_params.damping.get());
}

void ParticleObject::update(const State &state) {
  _velocity += _force;
  _position += _velocity;
}

void ParticleObject::outputFields(std::ostream &os) const {
  StandardWorldObject::outputFields(os);
  os << ", velocity: " << _velocity
      << ", force: " << _force;
}

static bool reboundVelocity(float *vel, float pos, float minPos, float maxPos) {
  float newPos = pos + *vel;
  if (newPos < minPos || newPos >= maxPos) {
    *vel *= -1;
    return true;
  } else {
    return false;
  }
}

AbstractReboundBehavior::AbstractReboundBehavior(const AbstractReboundBehavior::Params& params)
: _params(params) { }

bool AbstractReboundBehavior::updateEntity(ParticleObject &entity, const State &state) {
  ofVec3f lowBound = _params.lowValue.get();
  ofVec3f highBound = _params.highValue.get();

  bool changed = true;
  changed &= reboundVelocity(&entity._velocity.x, entity._position.x, lowBound.x, highBound.x);
  changed &= reboundVelocity(&entity._velocity.y, entity._position.y, lowBound.x, highBound.y);
  changed &= reboundVelocity(&entity._velocity.z, entity._position.z, lowBound.z, highBound.z);
  return changed;
}

