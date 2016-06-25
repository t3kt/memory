//
//  AnimationObject.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include "AnimationObject.h"

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

void AnimationObject::output(std::ostream &os) const {
  os << "Animation{id:" << id
    << "}";
}

DurationAction*
AnimationObject::createUpdaterAction(float time, ObjectManager<AnimationObject>& animationManager) {
  float now = time;
  float start = now + _delay;
  return new AnimationUpdater(start, start + _duration,
                              *this, animationManager);
}
