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

ObserversController::ObserversController(const ObserversController::Params& params,
                                         const Bounds& bounds,
                                         const State& state,
                                         SimulationEvents& events)
: _params(params)
, _bounds(bounds)
, _events(events)
, _spawnInterval(params.spawnInterval, state) {
}

void ObserversController::setup(const State &state, const ColorTheme& colors) {
  _thresholdRenderer = std::make_shared<ThresholdRenderer<ObserverEntity>>(_observers, _params.threshold, colors.getColor(ColorId::OBSERVER_THRESHOLD_CONNECTOR));
  _observerRenderer = std::make_shared<ObserverRenderer>(_params.renderer, colors, _observers);
  _observerConnectorRenderer = std::make_shared<ObserverObserverConnectorRenderer>(_params.connectorRenderer, colors.getColor(ColorId::OBSERVER_CONNECTOR), _observers);
  for (int i = 0; i < START_OBSERVERS; i++) {
    spawnObserver(state);
  }
}

void ObserversController::update(State &state) {
  _observers.performAction([&](std::shared_ptr<ObserverEntity> observer) {
    observer->update(state);
  });

  _observers.cullDeadObjects([&](std::shared_ptr<ObserverEntity> observer) {
    observer->detachConnections();
    ObserverEventArgs e(state, *observer);
    _events.observerDied.notifyListeners(e);
  });
  
  if (_spawnInterval.check(state)) {
    spawnObserver(state);
  }
  state.observerCount = _observers.size();

  _observerRenderer->update(state);
  _thresholdRenderer->update(state);
}

void ObserversController::draw(const State &state) {
  _observerRenderer->draw(state);
  _observerConnectorRenderer->draw(state);
  _thresholdRenderer->draw(state);
}

bool ObserversController::registerOccurrence(std::shared_ptr<OccurrenceEntity> occurrence) {
  bool connected = false;
  
  _observers.performAction([&] (std::shared_ptr<ObserverEntity> observer) {
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
  ofVec3f pos = _bounds.randomPoint();
  float life = _params.lifetime.getValue();
  auto observer = std::make_shared<ObserverEntity>(pos,
                                                   life,
                                                   state);
  observer->setVelocity(_params.initialVelocity.getValue());
  _observers.add(observer);
  ObserverEventArgs e(state, *observer);
  _events.observerSpawned.notifyListeners(e);
}

void ObserversController::spawnObservers(int count, const State& state) {
  for (int i = 0; i < count; ++i) {
    spawnObserver(state);
  }
}

void ObserversController::killObservers(int count) {
  int i = 0;
  for (auto& observer : _observers) {
    if (i >= count) {
      return;
    }
    observer->kill();
    i++;
  }
}
