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
  _observers.performAction([&](shared_ptr<ObserverEntity> observer) {
    observer->resetForce();
    observer->addDampingForce();
    observer->update(state);
  });

  _observers.cullDeadObjects([&](shared_ptr<ObserverEntity> observer) {
    ObserverEventArgs e(state, observer);
    observerDied.notifyListeners(e);
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
    float dist = occurrence->position().distance(observer->position());
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
  observer->setVelocity(ofVec3f(ofRandom(-0.01, 0.01),
                                ofRandom(-0.01, 0.01),
                                ofRandom(-0.01, 0.01)));
  _observers.add(observer);
  ObserverEventArgs e(state, observer);
  observerSpawned.notifyListeners(e);
  std::cout << "Spawned observer: " << *observer << std::endl;
}
