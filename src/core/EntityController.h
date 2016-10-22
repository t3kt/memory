//
//  EntityController.h
//  memory
//
//  Created by tekt on 8/5/16.
//
//

#pragma once

#include <memory>
#include "../control/Params.h"
#include "../core/Common.h"
#include "../core/Component.h"
#include "../core/ObjectManager.h"
#include "../core/SimulationEvents.h"

class Context;
class SimulationEvents;

// A controller which manages a set of entities of a particular type E.
template<typename E>
class EntityController
: public NonCopyable
, public ComponentBase {
public:
  using EntityPtr = std::shared_ptr<E>;

  EntityController(Context& context,
                   SimulationEvents& events,
                   ObjectManager<E>& entities)
  : _context(context)
  , _events(events)
  , _entities(entities) { }

  virtual void setup() {}

  ObjectManager<E>& entities() { return _entities; }
  const ObjectManager<E>& entities() const { return _entities; }

  // Kill the first N entities managed by the controller.
  void killEntities(int count) {
    int i = 0;
    for (auto& entity : _entities) {
      if (i >= count) {
        return;
      }
      entity->kill();
      i++;
    }
  }

  // Kill all entities managed by the controller.
  void killAllEntities() {
    for (auto& entity : _entities) {
      entity->kill();
    }
  }

  // Update each entity and remove the ones that die.
  virtual void update() {
    _entities.processAndCullObjects([&](EntityPtr& entity) {
      entity->update(_context.entityState);
      if (!entity->alive()) {
        _events.died<E>(*entity);
      }
    });
  }

protected:
  Context& _context;
  SimulationEvents& _events;
  ObjectManager<E>& _entities;
};
