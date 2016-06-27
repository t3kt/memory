//
//  ParticleObject.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/26/16.
//
//

#include "ParticleObject.h"

ParticleObject::ParticleObject(ofVec3f pos)
: StandardWorldObject()
, _velocity(0)
, _force(0)
, _damping(0) {
  _position = pos;
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
  addForce(_velocity.scale(-_damping));
}

void ParticleObject::update(const State &state) {
  _velocity += _force;
  _position += _velocity;
}
