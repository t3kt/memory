//
//  ObserversController.cpp
//

#include "../core/AnimationObject.h"
#include "../core/ObserversController.h"
#include "../core/OccurrenceEntity.h"
#include "../core/SimulationEvents.h"

ObserversController::ObserversController(const Params& params,
                                         const BoundsController& bounds,
                                         Context& context,
                                         SimulationEvents& events)
: EntityController(context,
                   events,
                   context.observers)
, _params(params)
, _bounds(bounds) { }

void ObserversController::setup() {
  _rateSpawner =
  _components.add<RateObserverSpawner>(_context,
                                       _params.rateSpawner,
                                       _bounds);
  _sickness =
  _components.add<ObserverSickness>(_context,
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
    case AppAction::KILL_ALL_OBSERVERS:
      killAllEntities();
      break;
    default:
      return false;
  }
  return true;
}
