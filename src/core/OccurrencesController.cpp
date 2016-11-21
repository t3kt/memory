//
//  OccurrencesController.cpp
//

#include "../core/OccurrencesController.h"
#include "../app/SimulationApp.h"
#include "../core/SimulationEvents.h"

OccurrencesController::OccurrencesController(const Params& params,
                                             const BoundsController& bounds,
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
  _components.add<OccurrenceSpawner>(_context,
                                     _params.rateSpawner,
                                     _bounds);
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
