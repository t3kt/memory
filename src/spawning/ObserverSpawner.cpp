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

void RateObserverSpawner::spawnEntities(Context &context, int count) {
  for (int i = 0; i < count; ++i) {
    auto pos = _bounds.randomPoint();
    auto life = _params.lifetime.getValue();
    auto observer = std::make_shared<ObserverEntity>(pos,
                                                     life,
                                                     context.state);
    observer->setVelocity(_params.initialVelocity.getValue());
    _controller.tryAddEntity(observer);
  }
}
