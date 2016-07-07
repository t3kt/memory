//
//  AnimationObject.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include "AnimationObject.h"
#include <ofMain.h>

class AnimationUpdater : public DurationAction {
public:
  AnimationUpdater(float start, float end,
                   AnimationObject& animation,
                   ObjectManager<AnimationObject>& animationManager)
  : DurationAction(start, end)
  , _animation(animation)
  , _animationManager(animationManager) { }
  
  virtual void call(float time, float percentage) override;
  
protected:
  virtual void start() override;
  virtual void end() override;
private:
  AnimationObject& _animation;
  ObjectManager<AnimationObject>& _animationManager;
};

void AnimationUpdater::call(float time, float percentage) {
  _animation._percentage = percentage;
}

void AnimationUpdater::start() {
  DurationAction::start();
  _animation.show();
}

void AnimationUpdater::end() {
  DurationAction::end();
  _animation.hide();
  _animationManager.eraseById(_animation.id);
}

AnimationObject::AnimationObject(const AnimationObject::Params& params, const State& state)
: WorldObject(state)
, _delay(params.delay())
, _duration(params.duration())
, _visible(true)
, _percentage(0) {
}

shared_ptr<DurationAction>
AnimationObject::createUpdaterAction(float time, ObjectManager<AnimationObject>& animationManager) {
  float now = time;
  float start = now + _delay;
  return std::make_shared<AnimationUpdater>(start, start + _duration,
                                            *this, animationManager);
}

ExpandingSphereAnimation::ExpandingSphereAnimation(ofVec3f position, const ExpandingSphereAnimation::Params& params, ofFloatColor color, const State& state)
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


