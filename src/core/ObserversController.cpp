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

  const std::string killCommandName = "killObs";
  AppSystem::get().commands()
  .registerCommand(killCommandName, "Kill Observers", [&](const CommandArgs& args) {
    if (args.empty()) {
      killAllEntities();
      return true;
    }
    if (args[0].type() == typeid(std::string) && args.get<std::string>(0) == "all") {
      killAllEntities();
      return true;
    }
    if (args[0].type() == typeid(int)) {
      killEntities(args.get<int>(0));
      return true;
    }
    return false;
  })
  .withButton(true)
  .withKeyMapping('-', CommandArgs({5}))
  .withKeyMapping('_', CommandArgs({100}));
}
