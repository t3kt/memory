//
//  Behavior.cpp
//  behavior
//
//  Created by tekt on 1/21/15.
//
//

#include "Behavior.h"
#include "State.h"
#include "Entity.h"

Behavior::Params::Params() {
  paramGroup.add(enabled.set("Enabled?", true));
}

void updateBehaviors(BehaviorList &behaviors,
                     Entity& entity,
                     State &state) {
  for (auto& behavior : behaviors) {
    behavior->update(entity, state);
  }
}
