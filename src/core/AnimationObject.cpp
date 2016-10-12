//
//  AnimationObject.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include <ofMain.h>
#include "../core/AnimationObject.h"

AnimationObject::AnimationObject(const Params& params,
                                 const ClockState& state)
: WorldObject(state)
, _duration(params.duration())
, _visible(true)
, _percentage(0)
, _startTime(state.localTime) {}

void AnimationObject::update(const ClockState &state) {
  float age = state.localTime - _startTime;
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
                                                   const ClockState& state)
: AnimationObject(params, state)
, _params(params)
, _color(color) {
  _position = position;
}

void ExpandingSphereAnimation::draw(const ClockState &state) {
  ofPushStyle();

  ofFloatColor color = _color;
  color.a *= _params.alpha.getLerped(percentage());
  ofSetColor(color);
  ofDrawSphere(_position, _params.radius.getLerped(percentage()));

  ofPopStyle();
}


