//
//  NodesController.cpp
//  memory
//
//  Created by tekt on 9/18/16.
//
//

#include "../core/NodesController.h"

NodesController::NodesController(Context& context,
                                 SimulationEvents& events)
: EntityController(context,
                   events,
                   context.nodes) { }
