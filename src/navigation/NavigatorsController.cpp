//
//  NavigatorsController.cpp
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#include "Context.h"
#include "NavigatorEntity.h"
#include "NavigatorsController.h"
#include "NavigatorState.h"
#include "ObserverEntity.h"

using NavEntityPtr = std::shared_ptr<NavigatorEntity>;

class ObserverNavSpawner
: public RateSpawner {
public:
  ObserverNavSpawner(NavigatorsController& controller,
                           const Params& params)
  : RateSpawner(params)
  , _controller(controller) { }

protected:
  void spawnEntities(Context& context, int count) override {
    //...
  }

private:
  NavigatorsController& _controller;
};

NavigatorsController::NavigatorsController(Context& context,
                                           Params& params)
: _context(context)
, _params(params)
, _navigators(context.navigators) { }

void NavigatorsController::setup() {
  _observerNavSpawner =
  std::make_shared<ObserverNavSpawner>(*this,
                                       _params.observerNavigatorSpawner);
}

void NavigatorsController::update() {
  _navigators.performAction([&](NavEntityPtr navigator) {
    if (!navigator->prevState() || !navigator->stateAlive()) {
      navigator->kill();
    } else {
      navigator->updateNextState(_context);
      const ofVec3f& targetPoint = navigator->targetPoint();
      const ofVec3f& currentPosition = navigator->position();
      ofVec3f diff = targetPoint - currentPosition;
      float dist = diff.length();
      diff.normalize();
      navigator->setVelocity(diff
                             * _params.moveRate.get()
                             * _context.state.timeDelta);
      navigator->updateVelocityAndPosition(_context.state, 1);

      diff = targetPoint - navigator->position();
      dist = diff.length();
      if (dist <= _params.reachRange.get()) {
        navigator->reachNextState(_context);
      }
    }
  });
  _navigators.cullDeadObjects([&](NavEntityPtr navigator) {
    //...
  });

  _observerNavSpawner->update(_context);
}

void NavigatorsController::draw() {
  //...
}

void NavigatorsController::spawnObserverNavigator(std::shared_ptr<ObserverEntity> entity) {
  auto startState = std::make_shared<ObserverNavState>(entity);
  auto navigator = std::make_shared<NavigatorEntity>(startState);
  _navigators.add(navigator);
  //...
}
