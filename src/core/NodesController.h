//
//  NodesController.h
//  memory
//
//  Created by tekt on 9/18/16.
//
//

#ifndef NodesController_h
#define NodesController_h

#include "../core/Context.h"
#include "../core/EntityController.h"
#include "../core/NodeEntity.h"

class SimulationEvents;

class NodesController
: public EntityController<NodeEntity> {
public:
  NodesController(Context& context,
                  SimulationEvents& events);
};

#endif /* NodesController_h */
