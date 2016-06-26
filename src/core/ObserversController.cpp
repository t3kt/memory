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

void ObserversController::Params::initPanel(ofxGuiGroup &panel) {
  entities.initPanel(panel);
  spawnInterval.initPanel(panel);
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
  _observers.cullDeadObjects([&](shared_ptr<ObserverEntity> observer) {
    EntityEventArgs<ObserverEntity> e(observer);
    ofNotifyEvent(observerDied, e);
  });
  
  if (_spawnInterval.check(state)) {
    spawnObserver(state);
  }
}

void ObserversController::draw(const State &state) {
  _observers.draw(state);
}

bool ObserversController::registerOccurrence(shared_ptr<OccurrenceEntity> occurrence) {
  bool connected = false;
  
  _observers.performAction([&] (shared_ptr<ObserverEntity> observer) {
    float dist = occurrence->position.distance(observer->position);
    if (dist <= occurrence->originalRadius()) {
      occurrence->addObserver(observer);
      observer->addOccurrence(occurrence);
      connected = true;
    }
    
  });
  return connected;
}

void ObserversController::spawnObserver(const State &state) {
  auto observer = ObserverEntity::spawn(_params.entities, state);
  _observers.add(observer);
  EntityEventArgs<ObserverEntity> e(observer);
  ofNotifyEvent(observerSpawned, e);
  std::cout << "Spawned observer: " << *observer << std::endl;
}
