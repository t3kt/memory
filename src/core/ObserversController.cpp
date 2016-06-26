//
//  ObserversController.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include "ObserversController.h"

const int START_OBSERVERS = 20;

ObserversController::Params::Params()
: ::Params("Observers")
, spawnInterval("Spawning") {
  add(entities);
  add(spawnInterval);
}

ObserversController::ObserversController(const ObserversController::Params& params, const State& state)
: _params(params)
, _spawnInterval(params.spawnInterval, state) {
}

void ObserversController::setup(const State &state) {
  for (int i = 0; i < START_OBSERVERS; i++) {
    spawnObserver(state);
  }
}

void ObserversController::update(const State &state) {
  _observers.update(state);
  _observers.extractDeadObjects();
  
  if (_spawnInterval.check(state)) {
    spawnObserver(state);
  }
}

void ObserversController::draw(const State &state) {
  _observers.draw(state);
}

void ObserversController::performAction(std::function<void (shared_ptr<ObserverEntity>)> action) {
  _observers.performAction(action);
}

void ObserversController::spawnObserver(const State &state) {
  auto observer = ObserverEntity::spawn(_params.entities, state);
  _observers.add(observer);
  std::cout << "Spawned observer: " << *observer << std::endl;
}
