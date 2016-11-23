//
//  ObserversController.cpp
//

#include "../app/AppSystem.h"
#include "../control/CommandsController.h"
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

  AppSystem::get().commands()
  .registerCommand("killObs", "Kill Observers", [&](const CommandArgs& args) {
    if (args.empty()) {
      killAllEntities();
      return true;
    }
    if (args.hasArg<std::string>(0, "all")) {
      killAllEntities();
      return true;
    }
    if (args.hasArgType<int>(0)) {
      killEntities(args.get<int>(0));
      return true;
    }
    return false;
  })
  .withButton(true)
  .withKeyMapping('-', CommandArgs({5}))
  .withKeyMapping('_', CommandArgs({100}));
}
