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
  _rateSpawner =
  std::make_shared<OccurrenceSpawner>(_context,
                                      _params.rateSpawner,
                                      _bounds);
}

void OccurrencesController::update() {
  EntityController::update();

  _rateSpawner->update();
}

bool OccurrencesController::performAction(AppAction action) {
  switch (action) {
    case AppAction::KILL_FEW_OCCURRENCES:
      killEntities(5);
      break;
    case AppAction::KILL_MANY_OCCURRENCES:
      killEntities(100);
      break;
    case AppAction::KILL_ALL_OCCURRENCES:
      killAllEntities();
      break;
    default:
      return false;
  }
  return true;
}
