//
//  TimingController.cpp
//

#include <ofConstants.h>
#include "../app/AppSystem.h"
#include "../control/CommandsController.h"
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

  AppSystem::get().commands()
  .registerCommand("toggleClockState", "Toggle Clock State", [&](const CommandArgs& args) {
   _rootClock->toggleState();
    return true;
  })
  .withButton(true)
  .withKeyMapping(' ');
}

void TimingController::update() {
  _clocks.update();
}
