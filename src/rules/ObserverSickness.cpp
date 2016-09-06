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
  auto mult = _params.lifetimeMult.get();
  for (int i = 0; i < count; ++i) {
    auto entity = getRandomEntity(_context.observers);
    if (entity && entity->alive() && !entity->sick()) {
      auto oldLife = entity->remainingLife();
      entity->setSick(true);
      entity->modifyRemainingLife(mult);
//      entity->capRemainingLifetime(3);
      log.logWarning([&](ofLog& log) {
        log << "Entity infected (id: "<< entity->id() <<") remaining life: " << oldLife << " -> " << entity->remainingLife();
      });
    }
  }
}
