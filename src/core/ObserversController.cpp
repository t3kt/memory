//
//  ObserversController.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include <ofLog.h>
#include "ObserversController.h"

const int START_OBSERVERS = 60;

ObserversController::Params::Params()
: ::Params("Observers")
, spawnInterval("Spawning")
, initialVelocity("Initial Velocity")
, occurrenceAttraction("Occurrence Attraction")
, threshold("Threshold") {
  add(entities);
  add(spawnInterval);
  add(initialVelocity
      .set(0, 0.01)
      .setParamRange(0, 0.1));
  add(occurrenceAttraction);
  add(threshold);
}

void ObserversController::Params::initPanel(ofxGuiGroup &panel) {
  entities.initPanel(panel);
  spawnInterval.initPanel(panel);
}

ObserversController::ObserversController(const ObserversController::Params& params, const Bounds& bounds, const State& state)
: _params(params)
, _bounds(bounds)
, _spawnInterval(params.spawnInterval, state) {
}

void ObserversController::setup(const State &state) {
  _reboundBehavior = std::make_shared<ReboundBehavior<ObserverEntity>>(_bounds);
  _reboundBehavior->entityRebounded += [&](ObserverEventArgs e) {
    observerRebounded.notifyListeners(e);
    ofLogNotice() << "Observer rebounded: " << e.entity();
  };
  _occurrenceAttraction = std::make_shared<ObserverOccurrenceAttraction>(_params.occurrenceAttraction);
  _thresholdRenderer = std::make_shared<ThresholdRenderer<ObserverEntity>>(_observers, _params.threshold);
  for (int i = 0; i < START_OBSERVERS; i++) {
    spawnObserver(state);
  }
  STATUS_COUNT = _status.registerLine("Observers:");
}

void ObserversController::update(const State &state) {
  _observers.performAction([&](shared_ptr<ObserverEntity> observer) {
    observer->resetForce();
    observer->addDampingForce();
    observer->update(state);
  });

  _observers.cullDeadObjects([&](shared_ptr<ObserverEntity> observer) {
    ObserverEventArgs e(state, *observer);
    observerDied.notifyListeners(e);
  });
  
  if (_spawnInterval.check(state)) {
    spawnObserver(state);
  }

  _thresholdRenderer->update(state);
  _status.setValue(STATUS_COUNT, ofToString(count()));
}

void ObserversController::draw(const State &state) {
  _observers.draw(state);
  _thresholdRenderer->draw(state);
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
  auto observer = ObserverEntity::spawn(_params.entities, _bounds, state);
  observer->setVelocity(_params.initialVelocity.getValue());
  observer->addBehavior(_reboundBehavior);
  observer->addBehavior(_occurrenceAttraction);
  _observers.add(observer);
  ObserverEventArgs e(state, *observer);
  observerSpawned.notifyListeners(e);
  ofLogNotice() << "Spawned observer: " << *observer;
}
