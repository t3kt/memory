//
//  OccurrencesController.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include "AppSystem.h"
#include "OccurrencesController.h"
#include "SimulationApp.h"

class IntervalOccurrenceSpawner
: public IntervalSpawner {
public:
  IntervalOccurrenceSpawner(OccurrencesController& controller)
  : IntervalSpawner(controller._params.spawner)
  , _controller(controller) { }

protected:
  void spawnEntities(Context& context) override {
    _controller.spawnRandomOccurrence();
  }

  OccurrencesController& _controller;
};

class RateOccurrenceSpawner
: public RateSpawner {
public:
  RateOccurrenceSpawner(OccurrencesController& controller)
  : RateSpawner(controller._params.rateSpawner)
  , _controller(controller) { }

protected:
  void spawnEntities(Context& context, int count) override {
    for (int i = 0; i < count; ++i) {
      _controller.spawnRandomOccurrence();
    }
  }

  OccurrencesController& _controller;
};

OccurrencesController::OccurrencesController(const Params& params,
                                             const Bounds& bounds,
                                             ObserversController& observers,
                                             Context& context,
                                             SimulationEvents& events)
: _params(params)
, _bounds(bounds)
, _events(events)
, _occurrences(context.occurrences)
, _observers(observers)
, _context(context) {
}

void OccurrencesController::setup(const ColorTheme& colors) {
  _renderer = std::make_shared<OccurrenceRenderer>(_params.renderer, colors, _occurrences);
  _observerOccurrenceConnectorRenderer = std::make_shared<ObserverOccurrenceConnectorRenderer>(_params.connectorRenderer, colors.getColor(ColorId::OCCURRENCE_OBSERVER_CONNECTOR), _occurrences);
  _occurrenceOccurrenceConnectorRenderer = std::make_shared<OccurrenceOccurrenceConnectorRenderer>(_params.occurrenceConnectorRenderer, colors.getColor(ColorId::OCCURRENCE_CONNECTOR), _occurrences);
  _spawner = std::make_shared<IntervalOccurrenceSpawner>(*this);
  _rateSpawner = std::make_shared<RateOccurrenceSpawner>(*this);

  registerAsActionHandler();
}

bool OccurrencesController::performAction(AppAction action) {
  const auto& state = AppSystem::get().simulation()->state();
  switch (action) {
    case AppAction::SPAWN_FEW_OCCURRENCES:
      spawnOccurrences(5);
      break;
    case AppAction::SPAWN_MANY_OCCURRENCES:
      spawnOccurrences(100);
      break;
    default:
      return false;
  }
  return true;
}

void OccurrencesController::update() {
  for (auto& occurrence : _occurrences) {
    if (!occurrence->hasConnectedObservers()) {
      occurrence->kill();
      occurrence->setAmountOfObservation(0);
      continue;
    }
    float amount = 0;
    float radius = 0;
    for (const auto& observer : occurrence->connectedObservers()) {
      amount += observer.second->getRemainingLifetimeFraction();
      float dist = occurrence->position().distance(observer.second->position());
      if (dist > radius) {
        radius = dist;
      }
    }
    occurrence->setAmountOfObservation(amount);
    occurrence->setActualRadius(radius);
  }
  _occurrences.cullDeadObjects([&](std::shared_ptr<OccurrenceEntity> occurrence) {
    occurrence->detachConnections();
    OccurrenceEventArgs e(_context.state, *occurrence);
    _events.occurrenceDied.notifyListeners(e);
  });

  _spawner->update(_context);
  _rateSpawner->update(_context);
  _context.state.occurrenceCount = _occurrences.size();
  _renderer->update(_context.state);
}

void OccurrencesController::draw() {
  _renderer->draw(_context.state);
  _observerOccurrenceConnectorRenderer->draw(_context.state);
  _occurrenceOccurrenceConnectorRenderer->draw(_context.state);
}

void OccurrencesController::spawnRandomOccurrence() {
  ofVec3f pos = _bounds.randomPoint();
  float radius = _params.radius.getValue();
  auto occurrence = std::make_shared<OccurrenceEntity>(pos,
                                                       radius,
                                                       _context.state);
  
  bool connected = _observers.registerOccurrence(occurrence);
  
  OccurrenceEventArgs e(_context.state, *occurrence);
  if (connected) {
    occurrence->setVelocity(_params.initialVelocity.getValue());
    _occurrences.add(occurrence);
    _events.occurrenceSpawned.notifyListeners(e);
  } else {
    _events.occurrenceSpawnFailed.notifyListeners(e);
  }
}

void OccurrencesController::spawnOccurrences(int count) {
  for (int i = 0; i < count; ++i) {
    spawnRandomOccurrence();
  }
}
