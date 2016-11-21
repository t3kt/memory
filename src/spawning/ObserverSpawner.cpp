//
//  ObserverSpawner.cpp
//

#include "../app/AppSystem.h"
#include "../app/SimulationApp.h"
#include "../control/Command.h"
#include "../control/CommandsController.h"
#include "../core/Context.h"
#include "../physics/BoundsController.h"
#include "../spawning/ObserverSpawner.h"

RateObserverSpawner::RateObserverSpawner(Context& context,
                                         const Params& params,
                                         const BoundsController& bounds)
: RateSpawner(context, params)
, _bounds(bounds)
, _events(AppSystem::get().simulation().getEvents()) { }

void RateObserverSpawner::setup() {
  AppSystem::get().simulation().commands()
  .registerCommand("spawnObs",
                   "Spawn Observer",
                   [&](const CommandArgs&) {
    spawnEntities(1);
    return true;
  });
}

void RateObserverSpawner::spawnEntities(int count) {
  for (int i = 0; i < count; ++i) {
    auto pos = _bounds.randomPoint();
    auto decay = _params.decayRate.getValue();
    auto observer = std::make_shared<ObserverEntity>(pos,
                                                     decay,
                                                     _context.entityState);
    observer->setVelocity(_params.initialVelocity.getValue());
    addEntity(observer);
  }
}

void RateObserverSpawner::addEntity(std::shared_ptr<ObserverEntity> entity) {
  _context.observers.add(entity);
  _events.spawned(*entity);
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
