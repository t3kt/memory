//
//  AnimationsController.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include "AnimationsController.h"
#include <ofEvent.h>

void AnimationsController::addAnimation(shared_ptr<AnimationObject> animation, const State& state) {
  _animations.add(animation);
  auto updater = animation->createUpdaterAction(state.time, _animations);
  addTimedAction(updater);
}

void AnimationsController::addTimedAction(shared_ptr<TimedAction> action) {
  _timedActions.add(action);
}

void AnimationsController::update(const State &state) {
  _animations.update(state);
  _timedActions.update(state.time);
}

void AnimationsController::draw(const State &state) {
  _animations.draw(state);
}

class ObserverDiedAnimation : public AnimationObject {
public:
  ObserverDiedAnimation(const ObserverEntity& observer) : _observer(observer) {}
private:
  const ObserverEntity& _observer;
};

void AnimationsController::attachTo(ObserversController &observers) {
//  ofAddListener(observers.observerDied, [&](ObserverEventArgs e) {
//    
//  });
  //...
}

void AnimationsController::attachTo(OccurrencesController &occurrences) {
  //...
}
