//
//  NavigatorsController.cpp
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#include <algorithm>
#include <ofMain.h>
#include <random>
#include "../app/AppAssets.h"
#include "../app/AppSystem.h"
#include "../core/Context.h"
#include "../navigation/NavigatorEntity.h"
#include "../navigation/NavigatorsController.h"
#include "../navigation/NavigatorState.h"
#include "../core/ObserverEntity.h"
#include "../core/SimulationEvents.h"

using NavEntityPtr = std::shared_ptr<NavigatorEntity>;

class ObserverNavSpawner
: public RateSpawner<> {
public:
  ObserverNavSpawner(NavigatorsController& controller,
                           const Params& params)
  : RateSpawner(params)
  , _controller(controller)
  , _randomGen(_randomDevice()) { }

protected:
  void spawnEntities(Context& context, int count) override {
    if (count == 1) {
      auto entity = getRandomEntity(context.observers);
      if (entity) {
        _controller.spawnObserverNavigator(entity);
      }
      return;
    }
    std::vector<std::shared_ptr<ObserverEntity>> entities(context.observers.begin(), context.observers.end());
    std::shuffle(entities.begin(), entities.end(), _randomGen);
    if (entities.size() < count) {
      count = entities.size();
    }
    for (int i = 0; i < count; ++i) {
      _controller.spawnObserverNavigator(entities[i]);
    }
  }

private:
  std::random_device _randomDevice;
  std::default_random_engine _randomGen;
  NavigatorsController& _controller;
};

NavigatorsController::NavigatorsController(Context& context,
                                           Params& params,
                                           SimulationEvents& events)
: _context(context)
, _params(params)
, _events(events)
, _navigators(context.navigators) { }

void NavigatorsController::setup() {
  _observerNavSpawner =
  std::make_shared<ObserverNavSpawner>(*this,
                                       _params.observerNavigatorSpawner);
}

void NavigatorsController::update() {
  if (!_context.state.running) {
    return;
  }
  _navigators.performAction([&](NavEntityPtr navigator) {
    if (!navigator->prevState() || !navigator->stateAlive()) {
      navigator->kill();
    } else {
      AppSystem::get().log().navigation().logNotice([&](ofLog& log) {
        log << "Updating navigator: " << *navigator;
      });
      navigator->updateNextState(_context);
      const ofVec3f& targetPoint = navigator->targetPoint();
      const ofVec3f& currentPosition = navigator->position();
      ofVec3f diff = targetPoint - currentPosition;
      float dist = diff.length();
      if (dist > _params.reachRange.get()) {
        diff.normalize();
        ofVec3f velocity = diff * _params.moveRate.get() * _context.state.timeDelta;
        if (velocity.length() > dist) {
          velocity.normalize();
          velocity *= dist;
        }
        navigator->setVelocity(velocity);
        navigator->updateVelocityAndPosition(_context.state, 1);

        diff = targetPoint - navigator->position();
        dist = diff.length();
      }
      if (navigator->nextState() && dist <= _params.reachRange.get()) {
        navigator->reachNextState(_context);
        NavigatorEventArgs e(SimulationEventType::NAVIGATOR_REACHED_LOCATION,
                             *navigator);
        _events.navigatorReachedLocation.notifyListeners(e);
      }
    }
    AppSystem::get().log().navigation().logNotice([&](ofLog& log) {
      log << "Updated navigator: " << *navigator;
    });
  });
  _navigators.cullDeadObjects([&](NavEntityPtr navigator) {
    NavigatorEventArgs e(SimulationEventType::NAVIGATOR_DIED,
                         *navigator);
    _events.navigatorDied.notifyListenersUntilHandled(e);
  });

  _observerNavSpawner->update(_context);

  _context.state.stats.navigatorCount = _navigators.size();
}

void NavigatorsController::draw() {
  ofPushStyle();
  ofSetColor(ofFloatColor(ofFloatColor::lightSteelBlue, 1.0));
  ofFill();
  for (const auto& navigator : _navigators) {
    if (!navigator->visible()) {
      continue;
    }
    ofPushMatrix();
    ofTranslate(navigator->position());
//    ofScale(ofVec3f(1));
    ofRotate(_context.time() * 8);
    AppAssets::navMarkerMesh().draw();
    ofPopMatrix();
  }
  ofPopStyle();
}

void NavigatorsController::spawnObserverNavigator(std::shared_ptr<ObserverEntity> entity) {
  auto startState = std::make_shared<ObserverNavState>(entity);
  auto navigator = std::make_shared<NavigatorEntity>(startState);
  _navigators.add(navigator);
  NavigatorEventArgs e(SimulationEventType::NAVIGATOR_SPAWNED,
                       *navigator);
  _events.navigatorSpawned.notifyListenersUntilHandled(e);
}
