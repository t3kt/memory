//
//  TimingController.cpp
//  memory
//
//  Created by tekt on 10/10/16.
//
//

#include "../core/Context.h"
#include "../core/TimingController.h"

void TimingController::setup() {
  _rootClock =
  _clocks.add<ClockNode>(_params.root,
                         _context.rootState,
                         nullptr);
  _clocks.add<ClockNode>(_params.entity,
                         _context.entityState,
                         _rootClock);
  _clocks.add<ClockNode>(_params.physics,
                         _context.physicsState,
                         _rootClock);
}

void TimingController::update() {
  _clocks.update();
}

bool TimingController::performAction(AppAction action) {
  switch (action) {
    case AppAction::TOGGLE_CLOCK_STATE:
      _rootClock->toggleState();
      return true;
    default:
      return false;
  }
}
