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
                                           SimulationEvents& events,
                                           Context& context)
: _params(params)
, _context(context)
, _colors(colors)
, _events(events)
, _animations(context.animations) {
}

void AnimationsController::setup() {
  attachToEvents();
}

void AnimationsController::addAnimation(std::shared_ptr<AnimationObject> animation) {
  AnimationEventArgs e(*animation);
  _events.animationSpawned.notifyListeners(e);
  
  _animations.add(animation);
}

void AnimationsController::update() {
  for (auto& animation : _animations) {
    animation->update(_context.state);
  }
  _animations.cullDeadObjects([&](std::shared_ptr<AnimationObject> animation) {
    AnimationEventArgs e(*animation);
    _events.animationDied.notifyListeners(e);
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
    auto observer = e.entity();
    auto animation = std::make_shared<ExpandingSphereAnimation>(observer.position(), _params.observerDied, _colors.getColor(ColorId::OBSERVER_DIED), _context.state);
    addAnimation(animation);
  };
  _events.occurrenceDied += [this](OccurrenceEventArgs e) {
    if (!_params.enabled() || !_params.occurrenceDied.enabled()) {
      return;
    }
    auto occurrence = e.entity();
    auto animation = std::make_shared<ExpandingSphereAnimation>(occurrence.position(), _params.observerDied, _colors.getColor(ColorId::OCCURRENCE_DIED), _context.state);
    addAnimation(animation);
  };
  _events.occurrenceSpawnFailed += [this](OccurrenceEventArgs e) {
    if (!_params.enabled() || !_params.occurrenceSpawnFailed.enabled()) {
      return;
    }
    auto occurrence = e.entity();
    auto animation = std::make_shared<ExpandingSphereAnimation>(occurrence.position(), _params.occurrenceSpawnFailed, _colors.getColor(ColorId::OCCURRENCE_SPAWN_FAILED), _context.state);
    addAnimation(animation);
  };
}
