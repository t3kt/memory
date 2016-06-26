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
, spawnInterval("Spawning") {
  add(entities);
  add(spawnInterval);
}

void OccurrencesController::Params::initPanel(ofxGuiGroup &panel) {
  entities.initPanel(panel);
  spawnInterval.initPanel(panel);
}

OccurrencesController::OccurrencesController(const OccurrencesController::Params& params, ObserversController& observers, const State& state)
: _params(params)
, _spawnInterval(params.spawnInterval, state)
, _observers(observers) {
}

void OccurrencesController::setup(const State &state) {
  for (int i = 0; i < START_OCCURRENCES; i++) {
    spawnOccurrence(state);
  }
}

void OccurrencesController::update(const State &state) {
  _occurrences.update(state);
  _occurrences.cullDeadObjects([&](shared_ptr<OccurrenceEntity> occurrence) {
    OccurrenceEventArgs e(state, occurrence);
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
  auto occurrence = OccurrenceEntity::spawn(_params.entities);
  
  bool connected = _observers.registerOccurrence(occurrence);
  
  if (connected) {
    std::cout << "Spawned occurrence: " << *occurrence << std::endl;
    _occurrences.add(occurrence);
    
    OccurrenceEventArgs e(state, occurrence);
    occurrenceSpawned.notifyListeners(e);
  } else {
    std::cout << "Nothing in range of occurrence: " << *occurrence << std::endl;
  }
}
