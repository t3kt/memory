//
//  ObserversController.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include "../core/AnimationObject.h"
#include "../core/ObserversController.h"
#include "../core/OccurrenceEntity.h"
#include "../core/SimulationEvents.h"

ObserversController::ObserversController(const Params& params,
                                         const Bounds& bounds,
                                         Context& context,
                                         SimulationEvents& events)
: EntityController(context,
                   events,
                   context.observers)
, _params(params)
, _bounds(bounds) { }

void ObserversController::setup() {
  _rateSpawner =
  std::make_shared<RateObserverSpawner>(_context,
                                        _params.rateSpawner,
                                        _bounds);
  _sickness =
  std::make_shared<ObserverSickness>(_context,
                                     _params.sickness);
}

bool ObserversController::performAction(AppAction action) {
  switch (action) {
    case AppAction::KILL_FEW_OBSERVERS:
      killEntities(5);
      break;
    case AppAction::KILL_MANY_OBSERVERS:
      killEntities(100);
      break;
    default:
      return false;
  }
  return true;
}

void ObserversController::update() {
  _sickness->update();
  _entities.processAndCullObjects([&](std::shared_ptr<ObserverEntity>& observer) {
    observer->update(_context.state);
    if (!observer->alive()) {
      observer->detachConnections();
      ObserverEventArgs e(SimulationEventType::OBSERVER_DIED,
                          *observer);
      _events.observerDied.notifyListeners(e);
    }
  });

  _rateSpawner->update();
  _context.state.observerCount = _entities.size();
}
