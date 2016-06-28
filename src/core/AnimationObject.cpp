//
//  AnimationObject.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include "AnimationObject.h"
#include <ofMain.h>

AnimationObject::Params::Params(std::string name)
: ::Params(name) {
  add(enabled.set("Enabled", true));
  add(delay.set("Delay", 0, 0, 2));
  add(duration.set("Duration", 1, 0, 8));
}

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

AnimationObject::AnimationObject(const AnimationObject::Params& params)
: _delay(params.delay.get())
, _duration(params.duration.get())
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

ExpandingSphereAnimation::Params::Params(std::string name)
: AnimationObject::Params(name)
, radius("Radius")
, color("Color")
, alpha("Alpha") {
  add(radius.setNames("Start", "End").setParamRange(0, 0.4).set(0, 0.2));
  add(alpha.setNames("Start", "End").setParamRange(0, 1).set(0, 1));
  add(color);
}

ExpandingSphereAnimation::ExpandingSphereAnimation(ofVec3f position, const ExpandingSphereAnimation::Params& params)
: AnimationObject(params)
, _params(params)
, _color(params.color.getValue()) {
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
