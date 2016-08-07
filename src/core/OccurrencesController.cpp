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

IntervalOccurrenceSpawner::IntervalOccurrenceSpawner(OccurrencesController& controller)
: IntervalSpawner(controller._params.spawner)
, _controller(controller) { }

void IntervalOccurrenceSpawner::spawnEntities(Context& context) {
  _controller.spawnRandomOccurrence();
}

RateOccurrenceSpawner::RateOccurrenceSpawner(OccurrencesController& controller)
: RateSpawner(controller._params.rateSpawner)
, _controller(controller) { }

void RateOccurrenceSpawner::spawnEntities(Context& context,
                                          int count) {
  for (int i = 0; i < count; ++i) {
    _controller.spawnRandomOccurrence();
  }
}

OccurrencesController::OccurrencesController(const Params& params,
                                             const Bounds& bounds,
                                             ObserversController& observers,
                                             Context& context,
                                             SimulationEvents& events)
: EntityController(params,
                   context,
                   events,
                   context.occurrences)
, _bounds(bounds)
, _observers(observers) { }

void OccurrencesController::setup() {
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
  for (auto& occurrence : _entities) {
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
  _entities.cullDeadObjects([&](std::shared_ptr<OccurrenceEntity> occurrence) {
    occurrence->detachConnections();
    OccurrenceEventArgs e(SimulationEventType::OCCURRENCE_DIED,
                          *occurrence);
    _events.occurrenceDied.notifyListeners(e);
  });

  _spawner->update(_context);
  _rateSpawner->update(_context);
  _context.state.occurrenceCount = _entities.size();
}

void OccurrencesController::draw() {
}

void OccurrencesController::spawnRandomOccurrence() {
  ofVec3f pos = _bounds.randomPoint();
  float radius = _params.radius.getValue();
  auto occurrence = std::make_shared<OccurrenceEntity>(pos,
                                                       radius,
                                                       _context.state);

  tryAddEntity(occurrence);
}

void OccurrencesController::spawnOccurrences(int count) {
  for (int i = 0; i < count; ++i) {
    spawnRandomOccurrence();
  }
}

bool OccurrencesController::tryAddEntity(std::shared_ptr<OccurrenceEntity> entity) {

  bool connected = _observers.registerOccurrence(entity);

  if (connected) {
    entity->setVelocity(_params.initialVelocity.getValue());
    _entities.add(entity);
    OccurrenceEventArgs e(SimulationEventType::OCCURRENCE_SPAWNED,
                          *entity);
    _events.occurrenceSpawned.notifyListeners(e);
    return true;
  } else {
    OccurrenceEventArgs e(SimulationEventType::OCCURRENCE_SPAWN_FAILED,
                          *entity);
    _events.occurrenceSpawnFailed.notifyListeners(e);
    return false;
  }
}
