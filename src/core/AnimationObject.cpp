//
//  AnimationObject.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include "AnimationObject.h"
#include <ofMain.h>

AnimationObject::AnimationObject(const Params& params,
                                 const State& state)
: _duration(params.duration())
, _visible(true)
, _percentage(0)
, _startTime(state.time) {}

void AnimationObject::update(const State &state) {
  float age = state.time - _startTime;
  if (age >= _duration) {
    _percentage = 1;
    kill();
  } else {
    _percentage = age / _duration;
  }
}

void AnimationObject::outputFields(std::ostream &os) const {
  WorldObject::outputFields(os);
  os << ", position: " << _position;
}

ExpandingSphereAnimation::ExpandingSphereAnimation(ofVec3f position,
                                                   const Params& params,
                                                   const ofFloatColor& color,
                                                   const State& state)
: AnimationObject(params, state)
, _params(params)
, _color(color) {
  _position = position;
}

void ExpandingSphereAnimation::draw(const State &state) {
  ofPushStyle();

  ofFloatColor color = _color;
  color.a *= _params.alpha.getLerped(percentage());
  ofSetColor(color);
  ofDrawSphere(_position, _params.radius.getLerped(percentage()));

  ofPopStyle();
}


