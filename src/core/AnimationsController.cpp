//
//  AnimationsController.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include "AnimationsController.h"
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include <ofMain.h>
#include <iostream>

AnimationsController::AnimationsController(const Params& params,
                                           const ColorTheme& colors,
                                           SimulationEvents& events)
: _params(params)
, _colors(colors)
, _events(events) {
}

void AnimationsController::setup() {
  attachToEvents();
}

void AnimationsController::addAnimation(shared_ptr<AnimationObject> animation, const State& state) {
  ofLogNotice() << "Adding animation: " << *animation;
  AnimationEventArgs e(state, *animation);
  _events.animationSpawned.notifyListeners(e);
  
  _animations.add(animation);
  auto updater = animation->createUpdaterAction(state.time, _animations);
  addTimedAction(updater);
}

void AnimationsController::addTimedAction(shared_ptr<TimedAction> action) {
  _timedActions.add(action);
}

void AnimationsController::update(State &state) {
  _animations.update(state);
  _timedActions.update(state.time);
  _animations.cullDeadObjects([&](shared_ptr<AnimationObject> animation) {
    AnimationEventArgs e(state, *animation);
    _events.animationDied.notifyListeners(e);
    ofLogNotice() << "Animation ended: " << *animation;
  });
  state.animationCount = _animations.size();
}

void AnimationsController::draw(const State &state) {
  _animations.draw(state);
}

void AnimationsController::attachToEvents() {
  _events.observerDied += [this](ObserverEventArgs e) {
    if (!_params.enabled() || !_params.observerDied.enabled()) {
      return;
    }
    auto observer = e.entity();
    auto animation = std::make_shared<ExpandingSphereAnimation>(observer.position(), _params.observerDied, _colors.getColor(ColorId::OBSERVER_DIED));
    addAnimation(animation, e.state);
    ofLogNotice() << "Adding observer died animation: " << *animation;
  };
  _events.occurrenceDied += [this](OccurrenceEventArgs e) {
    if (!_params.enabled() || !_params.occurrenceDied.enabled()) {
      return;
    }
    auto occurrence = e.entity();
    auto animation = std::make_shared<ExpandingSphereAnimation>(occurrence.position(), _params.observerDied, _colors.getColor(ColorId::OCCURRENCE_DIED));
    addAnimation(animation, e.state);
    ofLogNotice() << "Adding occurrence died animation: " << *animation;
  };
  _events.occurrenceSpawnFailed += [this](OccurrenceEventArgs e) {
    if (!_params.enabled() || !_params.occurrenceSpawnFailed.enabled()) {
      return;
    }
    auto occurrence = e.entity();
    auto animation = std::make_shared<ExpandingSphereAnimation>(occurrence.position(), _params.occurrenceSpawnFailed, _colors.getColor(ColorId::OCCURRENCE_SPAWN_FAILED));
    addAnimation(animation, e.state);
    ofLogNotice() << "Adding occurrence spawn failed animation: " << *animation;
  };
}
