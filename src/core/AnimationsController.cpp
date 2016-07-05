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
: ::Params("animations", "Animations") {
  add(_enabled
      .setKey("enabled")
      .setName("Enabled")
      .setValueAndDefault(true));
  add(observerDied
      .setKey("observerDied")
      .setName("Observer Died"));
  add(occurrenceDied
      .setKey("occurrenceDied")
      .setName("Occurrence Died"));
  add(occurrenceSpawnFailed
      .setKey("occurrenceSpawnFailed")
      .setName("Occurrence Spawn Failed"));
  occurrenceSpawnFailed.radius.setParamValuesAndDefaults(0, 0.01);
  occurrenceSpawnFailed.setDuration(1);
}

AnimationsController::AnimationsController(const AnimationsController::Params& params, const ColorTheme& colors)
: _params(params)
, _colors(colors) {
}

void AnimationsController::setup() {
}

void AnimationsController::addAnimation(shared_ptr<AnimationObject> animation, const State& state) {
  ofLogNotice() << "Adding animation: " << *animation;
  
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
    ofLogNotice() << "Animation ended: " << *animation;
  });
}

void AnimationsController::draw(const State &state) {
  _animations.draw(state);
}

void AnimationsController::attachTo(ObserversController &observers) {
  observers.observerDied += [&](ObserverEventArgs e) {
    if (!_params.enabled() || !_params.observerDied.enabled()) {
      return;
    }
    auto observer = e.entity();
    auto animation = std::make_shared<ExpandingSphereAnimation>(observer.position(), _params.observerDied, _colors.getColor(ColorId::OBSERVER_DIED), e.state);
    addAnimation(animation, e.state);
    ofLogNotice() << "Adding observer died animation: " << *animation;
  };
}

void AnimationsController::attachTo(OccurrencesController &occurrences) {
  occurrences.occurrenceDied += [&](OccurrenceEventArgs e) {
    if (!_params.enabled() || !_params.occurrenceDied.enabled()) {
      return;
    }
    auto occurrence = e.entity();
    auto animation = std::make_shared<ExpandingSphereAnimation>(occurrence.position(), _params.observerDied, _colors.getColor(ColorId::OCCURRENCE_DIED), e.state);
    addAnimation(animation, e.state);
    ofLogNotice() << "Adding occurrence died animation: " << *animation;
  };
  occurrences.occurrenceSpawnFailed += [&](OccurrenceEventArgs e) {
    if (!_params.enabled() || !_params.occurrenceSpawnFailed.enabled()) {
      return;
    }
    auto occurrence = e.entity();
    auto animation = std::make_shared<ExpandingSphereAnimation>(occurrence.position(), _params.occurrenceSpawnFailed, _colors.getColor(ColorId::OCCURRENCE_SPAWN_FAILED), e.state);
    addAnimation(animation, e.state);
    ofLogNotice() << "Adding occurrence spawn failed animation: " << *animation;
  };
}
