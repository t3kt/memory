//
//  ObserverSpawner.cpp
//

#include "../app/AppSystem.h"
#include "../app/SimulationApp.h"
#include "../control/Command.h"
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
  registerSpawnCommand("spawnObs", "Spawn Observer")
  .withButton(true)
  .withKeyMapping('0', CommandArgs{5})
  .withKeyMapping(')', CommandArgs{100})
  .withKeyMapping('%', CommandArgs{1000});
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
