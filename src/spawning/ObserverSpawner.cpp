//
//  ObserverSpawner.cpp
//  memory
//
//  Created by tekt on 8/6/16.
//
//

#include "../core/Bounds.h"
#include "../core/Context.h"
#include "../core/ObserversController.h"
#include "../spawning/ObserverSpawner.h"

void RateObserverSpawner::spawnEntities(int count) {
  for (int i = 0; i < count; ++i) {
    auto pos = _bounds.randomPoint();
    auto decay = _params.decayRate.getValue();
    auto observer = std::make_shared<ObserverEntity>(pos,
                                                     decay,
                                                     _context.state);
    observer->setVelocity(_params.initialVelocity.getValue());
    _controller.tryAddEntity(observer);
  }
}

bool RateObserverSpawner::performAction(AppAction action) {
  switch (action) {
    case AppAction::SPAWN_FEW_OBSERVERS:
      spawnEntities(5);
      return true;
    case AppAction::SPAWN_MANY_OBSERVERS:
      spawnEntities(100);
      return true;
    case AppAction::SPAWN_TONS_OF_OBSERVERS:
      spawnEntities(4000);
      return true;
    default:
      return false;
  }
}
