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
#include "AppActions.h"
#include "ObjectManager.h"
#include "Params.h"
#include "SimulationEvents.h"

class Context;

class AbstractEntityController
: public AppActionHandler {
public:
  class Params : public ::Params {
  };

  virtual void setup() = 0;
  virtual void update() = 0;
  virtual void draw() {};
};

template<typename E, typename P>
class EntityController
: public AbstractEntityController {
public:
  using Params = P;

  EntityController(const P& params,
                   Context& context,
                   SimulationEvents& events,
                   ObjectManager<E>& entities)
  : _params(params)
  , _context(context)
  , _events(events)
  , _entities(entities) { }

  ObjectManager<E>& entities() { return _entities; }
  const ObjectManager<E>& entities() const { return _entities; }

//  virtual bool tryAddEntity(std::shared_ptr<E> entity) = 0;

protected:
  const P& _params;
  Context& _context;
  SimulationEvents& _events;
  ObjectManager<E>& _entities;
};

#endif /* EntityController_h */
