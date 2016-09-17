//
//  AnimationsController.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include <iostream>
#include <ofMain.h>
#include "../core/AnimationsController.h"
#include "../app/AppSystem.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"
#include "../core/SimulationEvents.h"

AnimationsController::AnimationsController(const Params& params,
                                           SimulationEvents& events,
                                           Context& context)
: _params(params)
, _context(context)
, _colors(AppSystem::get().params()->colors)
, _events(events)
, _animations(context.animations) {
}

void AnimationsController::setup() {
  attachToEvents();
}

void AnimationsController::addAnimation(std::shared_ptr<AnimationObject> animation) {
  AnimationEventArgs e(SimulationEventType::ANIMATION_SPAWNED,
                       *animation);
  _events.animationSpawned.notifyListeners(e);
  
  _animations.add(animation);
}

void AnimationsController::update() {
  _animations.processAndCullObjects([&](std::shared_ptr<AnimationObject> animation) {
    animation->update(_context.state);
    if (!animation->alive()) {
      AnimationEventArgs e(SimulationEventType::ANIMATION_DIED,
                           *animation);
      _events.animationDied.notifyListeners(e);
    }
  });
  _context.state.animationCount = _animations.size();
}

void AnimationsController::draw() {
  for (auto& animation : _animations) {
    if (animation->visible()) {
      animation->draw(_context.state);
    }
  }
}

void AnimationsController::attachToEvents() {
  _events.observerDied += [this](ObserverEventArgs e) {
    if (!_params.enabled() || !_params.observerDied.enabled()) {
      return;
    }
    auto observer = e.value();
    auto animation = std::make_shared<ExpandingSphereAnimation>(observer.position(), _params.observerDied, _colors.observerDied.get(), _context.state);
    addAnimation(animation);
  };
  _events.occurrenceDied += [this](OccurrenceEventArgs e) {
    if (!_params.enabled() || !_params.occurrenceDied.enabled()) {
      return;
    }
    auto occurrence = e.value();
    auto animation = std::make_shared<ExpandingSphereAnimation>(occurrence.position(), _params.observerDied, _colors.occurrenceDied.get(), _context.state);
    addAnimation(animation);
  };
  _events.occurrenceSpawnFailed += [this](OccurrenceEventArgs e) {
    if (!_params.enabled() || !_params.occurrenceSpawnFailed.enabled()) {
      return;
    }
    auto occurrence = e.value();
    auto animation = std::make_shared<ExpandingSphereAnimation>(occurrence.position(), _params.occurrenceSpawnFailed, _colors.occurrenceSpawnFailed.get(), _context.state);
    addAnimation(animation);
  };
}
