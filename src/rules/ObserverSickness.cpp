//
//  ObserverSickness.cpp
//  memory
//
//  Created by tekt on 9/5/16.
//
//

#include "../app/AppSystem.h"
#include "../core/Logging.h"
#include "../core/ObserverEntity.h"
#include "../rules/ObserverSickness.h"

void ObserverSickness::update() {
  if (!_params.enabled.get()) {
    return;
  }
  int count = _scheduler.query();
  auto& log = AppSystem::get().log().observer();
  auto addDecay = _params.decayRateAmount.get();
  for (int i = 0; i < count; ++i) {
    auto entity = _context.observers.getRandomEntity();
    if (entity && entity->alive() && !entity->sick()) {
      auto oldDecayRate = entity->getDecayRate();
      entity->setDecayRate(oldDecayRate + addDecay);
      entity->setSick(true);
      log.logWarning([&](ofLog& log) {
        log << "Entity infected (id: "<< entity->id() <<") decay rate: " << oldDecayRate << " -> " << entity->getDecayRate();
      });
    }
  }
}
