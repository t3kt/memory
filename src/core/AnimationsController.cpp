//
//  AnimationsController.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include "AnimationsController.h"
#include <ofMain.h>
#include <iostream>

AnimationsController::Params::Params()
: ::Params("Animations")
, observerDied("Observer Died")
, occurrenceDied("Occurrence Died") {
  add(enabled.set("Enabled", true));
  add(observerDied);
  add(occurrenceDied);
}

AnimationsController::AnimationsController(const AnimationsController::Params& params)
: _params(params) {
}

void AnimationsController::addAnimation(shared_ptr<AnimationObject> animation, const State& state) {
  std::cout << "Adding animation: " << *animation << std::endl;
  
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
  _animations.cullDeadObjects([&](shared_ptr<AnimationObject> animation) {
    std::cout << "Animation ended: " << *animation << std::endl;
  });
}

void AnimationsController::draw(const State &state) {
  _animations.draw(state);
}

void AnimationsController::attachTo(ObserversController &observers) {
  observers.observerDied += [&](ObserverEventArgs e) {
    if (!_params.enabled.get() || !_params.observerDied.enabled.get()) {
      return;
    }
    auto observer = e.entity();
    auto animation = std::make_shared<ExpandingSphereAnimation>(observer->position(), _params.observerDied);
    addAnimation(animation, e.state);
    std::cout << "Adding observer died animation: " << *animation << std::endl;
  };
  //...
}

void AnimationsController::attachTo(OccurrencesController &occurrences) {
  occurrences.occurrenceDied += [&](OccurrenceEventArgs e) {
    if (!_params.enabled.get() || !_params.occurrenceDied.enabled.get()) {
      return;
    }
    auto occurrence = e.entity();
    auto animation = std::make_shared<ExpandingSphereAnimation>(occurrence->position(), _params.observerDied);
    addAnimation(animation, e.state);
    std::cout << "Adding occurrence died animation: " << *animation << std::endl;
  };
  //...
}
