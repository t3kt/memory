//
//  ParticleObject.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/26/16.
//
//

#include "ParticleObject.h"

ParticleObject::Params::Params()
: ::Params() {
  add(_damping
      .setKey("damping")
      .setName("Damping")
      .setValueAndDefault(0.01)
      .setRange(0, 0.1));
  add(_speed
      .setKey("speed")
      .setName("Speed")
      .setValueAndDefault(1)
      .setRange(0, 10));
}

AbstractEntityAttraction::Params::Params()
: ::Params() {
  add(_enabled
      .setKey("enabled")
      .setName("Enabled")
      .setValueAndDefault(true));
  add(distanceBounds
      .setKey("distanceBounds")
      .setName("Distance Bounds")
      .setParamValuesAndDefaults(0.04, 0.3)
      .setParamRanges(0, 1));
  add(forceRange
      .setKey("forceRange")
      .setName("Force Range")
      .setParamValuesAndDefaults(0.0001, 0)
      .setParamRanges(-0.005, 0.005));
}

ParticleObject::ParticleObject(ofVec3f pos, const ParticleObject::Params& params)
: WorldObject()
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

AbstractSpatialNoiseForce::Params::Params()
: ::Params() {
  add(_enabled
      .setKey("enabled")
      .setName("Enabled")
      .setValueAndDefault(true));
  add(_scale
      .setKey("scale")
      .setName("Scale")
      .setValueAndDefault(1)
      .setRange(0, 4));
  add(_rate
      .setKey("rate")
      .setName("Rate")
      .setValueAndDefault(0.1)
      .setRange(0, 0.5));
  add(_magnitude
      .setKey("magnitude")
      .setName("Magnitude")
      .setValueAndDefault(0.0001)
      .setRange(0, 0.005));
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

