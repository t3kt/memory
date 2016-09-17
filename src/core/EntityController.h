//
//  EntityController.h
//  memory
//
//  Created by tekt on 8/5/16.
//
//

#ifndef EntityController_h
#define EntityController_h

#include <memory>
#include "../core/ObjectManager.h"
#include "../core/Params.h"
#include "../core/SimulationEvents.h"

class Context;
class SimulationEvents;

class AbstractEntityController {
public:
  virtual void setup() = 0;
  virtual void update() = 0;
};

template<typename E>
class EntityController
: public AbstractEntityController {
public:
  using EntityPtr = std::shared_ptr<E>;
  using EntityEvent = SimulationEvent<E>;
  using EntityEventArgs = SimulationEventArgs<E>;

  EntityController(Context& context,
                   SimulationEvents& events,
                   ObjectManager<E>& entities)
  : _context(context)
  , _events(events)
  , _entities(entities) { }

  ObjectManager<E>& entities() { return _entities; }
  const ObjectManager<E>& entities() const { return _entities; }

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

  virtual void update() {
    _entities.processAndCullObjects([&](EntityPtr& entity) {
      entity->update(_context.state);
      if (!entity->alive()) {
        EntityEventArgs e(diedEventType<E>(), *entity);
        _events.died<E>().notifyListeners(e);
      }
    });
  }

protected:
  Context& _context;
  SimulationEvents& _events;
  ObjectManager<E>& _entities;
};

#endif /* EntityController_h */
