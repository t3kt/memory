//
//  AnimationObject.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include <ofMain.h>
#include "../core/AnimationObject.h"

AnimationObject::AnimationObject(ofVec3f pos,
                                 const Params& params,
                                 const ClockState& state)
: ParticleObject(pos, state)
, _duration(params.duration())
, _visible(true)
, _percentage(0)
, _startTime(state.localTime) {}

void AnimationObject::update(const ClockState &state) {
  ParticleObject::update(state);
  float age = state.localTime - _startTime;
  if (age >= _duration) {
    _percentage = 1;
    kill();
  } else {
    _percentage = age / _duration;
  }
}

ExpandingSphereAnimation::ExpandingSphereAnimation(ofVec3f position,
                                                   const Params& params,
                                                   const ofFloatColor& color,
                                                   const ClockState& state)
: AnimationObject(position, params, state)
, _params(params)
, _color(color) {}

void ExpandingSphereAnimation::draw(const ClockState &state) {
  ofPushStyle();

  ofFloatColor color = _color;
  color.a *= _params.alpha.getLerped(percentage());
  ofSetColor(color);
  ofDrawSphere(_position, _params.radius.getLerped(percentage()));

  ofPopStyle();
}


