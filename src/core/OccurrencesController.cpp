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
: ::Params() {
  add(entities
      .setKey("entities")
      .setName("Occurrences"));
  add(spawnInterval
      .setKey("spawnInterval")
      .setName("Spawning"));
  add(initialVelocity
      .setKey("initialVelocity")
      .setName("Initial Velocity")
      .setParamValuesAndDefaults(0, 0.01)
      .setParamRanges(0, 0.1));
  add(observerAttraction
      .setKey("observerAttraction")
      .setName("Observer Attraction"));
  add(spatialNoiseForce
      .setKey("spatialNoiseForce")
      .setName("Spatial Noise Force"));
  add(renderer
      .setKey("renderer")
      .setName("Renderer"));
  add(connectorRenderer
      .setKey("connectorRenderer")
      .setName("Connector Renderer"));
  observerAttraction.setEnabled(false);
  spatialNoiseForce.setEnabled(false);
}

OccurrencesController::OccurrencesController(const OccurrencesController::Params& params, const Bounds& bounds, ObserversController& observers, const State& state, const ColorTheme& colors)
: _params(params)
, _bounds(bounds)
, _colors(colors)
, _spawnInterval(params.spawnInterval, state)
, _observers(observers) {
}

void OccurrencesController::setup(const State &state) {
  _reboundBehavior = std::make_shared<ReboundBehavior<OccurrenceEntity>>(_bounds);
  _reboundBehavior->entityRebounded += [&](OccurrenceEventArgs e) {
//    ofLogNotice() << "Occurrence rebounded: " << e.entity();
  };
  _observerAttraction = std::make_shared<OccurrenceObserverAttraction>(_params.observerAttraction);
  _spatialNoiseForce = std::make_shared<SpatialNoiseForce<OccurrenceEntity>>(_params.spatialNoiseForce);
  _renderer = std::make_shared<OccurrenceRenderer>(_params.renderer, _colors, _occurrences);
  _observerOccurrenceConnectorRenderer = std::make_shared<ObserverOccurrenceConnectorRenderer>(_params.connectorRenderer, _colors.getColor(ColorId::OCCURRENCE_CONNECTOR), _occurrences);
  for (int i = 0; i < START_OCCURRENCES; i++) {
    spawnOccurrence(state);
  }
  STATUS_COUNT = _status.registerLine("Occurrences:");
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
  _status.setValue(STATUS_COUNT, ofToString(count()));
}

void OccurrencesController::draw(const State &state) {
  _renderer->draw(state);
  _observerOccurrenceConnectorRenderer->draw(state);
}

void OccurrencesController::spawnOccurrence(const State &state) {
  auto occurrence = OccurrenceEntity::spawn(_params.entities, _bounds, state, _colors);
  
  bool connected = _observers.registerOccurrence(occurrence);
  
  OccurrenceEventArgs e(state, *occurrence);
  if (connected) {
    occurrence->setVelocity(_params.initialVelocity.getValue());
    occurrence->addBehavior(_reboundBehavior);
    occurrence->addBehavior(_observerAttraction);
    occurrence->addBehavior(_spatialNoiseForce);
    ofLogNotice() << "Spawned occurrence: " << *occurrence;
    _occurrences.add(occurrence);
    occurrenceSpawned.notifyListeners(e);
  } else {
    ofLogNotice() << "Nothing in range of occurrence: " << *occurrence;
    occurrenceSpawnFailed.notifyListeners(e);
  }
}

void OccurrencesController::spawnOccurrences(int count,
                                             const State &state) {
  for (int i = 0; i < count; ++i) {
    spawnOccurrence(state);
  }
}
