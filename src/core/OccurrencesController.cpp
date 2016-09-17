//
//  OccurrencesController.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

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
  _rateSpawner = std::make_shared<RateOccurrenceSpawner>(_context,
                                                         _params.rateSpawner,
                                                         _bounds,
                                                         *this);
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
    for (const auto& observer : occurrence->getConnectedObservers()) {
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

  _rateSpawner->update();
  _context.state.occurrenceCount = _entities.size();
}

void OccurrencesController::spawnOccurrences(int count) {
  if (_rateSpawner->spawnNow(count)) {
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
