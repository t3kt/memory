//
//  OccurrencesController.cpp
//

#include "../app/AppSystem.h"
#include "../app/SimulationApp.h"
#include "../control/CommandsController.h"
#include "../core/OccurrencesController.h"
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

  AppSystem::get().commands()
  .registerCommand("killOcc", "Kill Occurrences", [&](const CommandArgs& args) {
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
  .withButton(false)
  .withKeyMapping('=', CommandArgs({5}))
  .withKeyMapping('+', CommandArgs({100}));
}
