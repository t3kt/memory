//
//  OccurrencesController.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include "../app/AppSystem.h"
#include "../core/OccurrencesController.h"
#include "../app/SimulationApp.h"
#include "../core/SimulationEvents.h"

OccurrencesController::OccurrencesController(const Params& params,
                                             const Bounds& bounds,
                                             ObserversController& observers,
                                             Context& context,
                                             SimulationEvents& events)
: EntityController(context,
                   events,
                   context.occurrences)
, _params(params)
, _bounds(bounds)
, _observers(observers) { }

void OccurrencesController::setup() {
  _spawner = std::make_shared<IntervalOccurrenceSpawner>(_params.spawner,
                                                         _bounds,
                                                         *this);
  _rateSpawner = std::make_shared<RateOccurrenceSpawner>(_params.rateSpawner,
                                                         _bounds,
                                                         *this);

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
    case AppAction::SPAWN_TONS_OF_OCCURRENCES:
      spawnOccurrences(4000);
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
  _context.state.stats.occurrenceCount = _entities.size();
}

void OccurrencesController::draw() {
}

void OccurrencesController::spawnOccurrences(int count) {
  if (_spawner->spawnNow(_context, count)) {
    return;
  }
  if (_rateSpawner->spawnNow(_context, count)) {
    return;
  }
}

bool OccurrencesController::tryAddEntity(std::shared_ptr<OccurrenceEntity> entity) {

  bool connected = _observers.registerOccurrence(entity);

  if (connected) {
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
