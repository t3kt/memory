//
//  ParticleObject.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/26/16.
//
//

#include "ParticleObject.h"

ParticleObject::ParticleObject(ofVec3f pos, const ParticleObject::Params& params, const State& state)
: WorldObject(state)
, _velocity(0)
, _force(0)
, _params(params) {
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
  addForce(_velocity * -_params.damping());
}

void ParticleObject::update(const State &state) {
  if (state.timeDelta > 0) {
    _velocity += _force * state.timeDelta;
    _position += _velocity * _params.speed() * state.timeDelta;
  }
}

void ParticleObject::outputFields(std::ostream &os) const {
  WorldObject::outputFields(os);
  os << ", velocity: " << _velocity
      << ", force: " << _force;
}

AbstractReboundBehavior::AbstractReboundBehavior(const Bounds& bounds)
: _bounds(bounds) { }

bool AbstractReboundBehavior::updateEntity(ParticleObject &entity, const State &state) {
  return _bounds.reflect(&entity._velocity, &entity._position);
}

const ofVec4f SPATIAL_NOISE_Y_OFFSET = ofVec4f(100);
const ofVec4f SPATIAL_NOISE_Z_OFFSET = ofVec4f(200);

ofVec3f AbstractSpatialNoiseForce::getForce(ofVec3f position,
                                            const State &state) const {
  ofVec4f noisePos = ofVec4f(position * _params.scale());
  noisePos.w = state.time * _params.rate();

  ofVec3f force(ofSignedNoise(noisePos),
                ofSignedNoise(noisePos + SPATIAL_NOISE_Y_OFFSET),
                ofSignedNoise(noisePos + SPATIAL_NOISE_Z_OFFSET));

  return force * _params.magnitude();
}

