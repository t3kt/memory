//
//  OccurrencesController.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include "OccurrencesController.h"

const int START_OCCURRENCES = 5;

OccurrencesController::Params::Params()
: ::Params("Occurrences")
, spawnInterval("Spawning")
, initialVelocity("Initial Velocity")
, observerAttraction("Observer Attraction") {
  add(entities);
  add(spawnInterval);
  add(initialVelocity.set(0, 0)
      .setParamRange(0, 0.1));
  add(observerAttraction);
  observerAttraction.enabled.set(false);
}

void OccurrencesController::Params::initPanel(ofxGuiGroup &panel) {
  entities.initPanel(panel);
  spawnInterval.initPanel(panel);
}

OccurrencesController::OccurrencesController(const OccurrencesController::Params& params, const Bounds& bounds, ObserversController& observers, const State& state)
: _params(params)
, _bounds(bounds)
, _spawnInterval(params.spawnInterval, state)
, _observers(observers) {
}

void OccurrencesController::setup(const State &state) {
  _reboundBehavior = std::make_shared<ReboundBehavior<OccurrenceEntity>>(_bounds);
  _reboundBehavior->entityRebounded += [&](OccurrenceEventArgs e) {
//    ofLogNotice() << "Occurrence rebounded: " << e.entity();
  };
  _observerAttraction = std::make_shared<OccurrenceObserverAttraction>(_params.observerAttraction);
  for (int i = 0; i < START_OCCURRENCES; i++) {
    spawnOccurrence(state);
  }
}

void OccurrencesController::update(const State &state) {
  _occurrences.update(state);
  _occurrences.cullDeadObjects([&](shared_ptr<OccurrenceEntity> occurrence) {
    OccurrenceEventArgs e(state, *occurrence);
    occurrenceDied.notifyListeners(e);
  });
  
  if (_spawnInterval.check(state)) {
    spawnOccurrence(state);
  }
}

void OccurrencesController::draw(const State &state) {
  _occurrences.draw(state);
}

void OccurrencesController::spawnOccurrence(const State &state) {
  auto occurrence = OccurrenceEntity::spawn(_params.entities, _bounds, state);
  
  bool connected = _observers.registerOccurrence(occurrence);
  
  OccurrenceEventArgs e(state, *occurrence);
  if (connected) {
    occurrence->setVelocity(_params.initialVelocity.getValue());
    occurrence->addBehavior(_reboundBehavior);
    occurrence->addBehavior(_observerAttraction);
    ofLogNotice() << "Spawned occurrence: " << *occurrence;
    _occurrences.add(occurrence);
    occurrenceSpawned.notifyListeners(e);
  } else {
    ofLogNotice() << "Nothing in range of occurrence: " << *occurrence;
    occurrenceSpawnFailed.notifyListeners(e);
  }
}
