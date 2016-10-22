//
//  TimingController.cpp
//  memory
//
//  Created by tekt on 10/10/16.
//
//

#include "../app/AppSystem.h"
#include "../core/Context.h"
#include "../core/TimingController.h"

class PauseHandler {
public:
  PauseHandler(MemoryAppParameters& appParams)
  : _paused(appParams.core.timing.root.paused)
  , _wasPaused(appParams.core.timing.root.paused.get()) {
    if (!_wasPaused) {
      _paused.set(true);
    }
  }

  ~PauseHandler() {
    _paused.set(_wasPaused);
  }

private:
  TParam<bool>& _paused;
  bool _wasPaused;
};

bool TimingController::doWhilePaused(std::function<bool(void)> action) {
  PauseHandler pauseHandler(AppSystem::get().params());
  return action();
}

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
