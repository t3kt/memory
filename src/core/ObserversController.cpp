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

ObserversController::ObserversController(const ObserversController::Params& params, const Bounds& bounds, const State& state)
: _params(params)
, _bounds(bounds)
, _spawnInterval(params.spawnInterval, state) {
}

void ObserversController::setup(const State &state, const ColorTheme& colors) {
  _thresholdRenderer = std::make_shared<ThresholdRenderer<ObserverEntity>>(_observers, _params.threshold, colors.getColor(ColorId::OBSERVER_THRESHOLD_CONNECTOR));
  _observerRenderer = std::make_shared<ObserverRenderer>(_params.renderer, colors, _observers);
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
    ObserverEventArgs e(state, *observer);
    observerDied.notifyListeners(e);
  });
  
  if (_spawnInterval.check(state)) {
    spawnObserver(state);
  }

  _observerRenderer->update(state);
  _thresholdRenderer->update(state);
}

void ObserversController::draw(const State &state) {
  _observerRenderer->draw(state);
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
  _observers.add(observer);
  ObserverEventArgs e(state, *observer);
  observerSpawned.notifyListeners(e);
  ofLogNotice() << "Spawned observer: " << *observer;
}

void ObserversController::spawnObservers(int count, const State& state) {
  for (int i = 0; i < count; ++i) {
    spawnObserver(state);
  }
}
