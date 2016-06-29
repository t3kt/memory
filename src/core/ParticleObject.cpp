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
  add(damping.set("Damping", 0.01, 0, 0.1));
}

AbstractEntityAttraction::Params::Params(std::string name)
: ::Params(name)
, distanceBounds("Distance Bounds")
, forceRange("Force Range") {
  add(enabled.set("Enabled", true));
  add(distanceBounds.set(0.04, 0.3).setParamRange(0, 1));
  add(forceRange.set(0.0001, 0).setParamRange(-0.005, 0.005));
}

ParticleObject::ParticleObject(ofVec3f pos, const ParticleObject::Params& params)
: StandardWorldObject()
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

AbstractReboundBehavior::AbstractReboundBehavior(const Bounds& bounds)
: _bounds(bounds) { }

bool AbstractReboundBehavior::updateEntity(ParticleObject &entity, const State &state) {
  return _bounds.reflect(&entity._velocity, &entity._position);
}

AbstractSpatialNoiseForce::Params::Params(std::string name)
: ::Params(name) {
  add(enabled.set("Enabled", true));
  add(scale.set("Scale", 1, 0, 4));
  add(rate.set("Rate", 0.1, 0, 0.5));
  add(magnitude.set("Magnitude", 0.0001, 0, 0.005));
}

const ofVec4f SPATIAL_NOISE_Y_OFFSET = ofVec4f(100);
const ofVec4f SPATIAL_NOISE_Z_OFFSET = ofVec4f(200);

ofVec3f AbstractSpatialNoiseForce::getForce(ofVec3f position,
                                            const State &state) const {
  ofVec4f noisePos = ofVec4f(position * _params.scale.get());
  noisePos.w = state.time * _params.rate.get();

  ofVec3f force(ofSignedNoise(noisePos),
                ofSignedNoise(noisePos + SPATIAL_NOISE_Y_OFFSET),
                ofSignedNoise(noisePos + SPATIAL_NOISE_Z_OFFSET));

  return force * _params.magnitude.get();
}

