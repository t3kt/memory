//
//  DampingBehavior.h
//  memory
//
//  Created by tekt on 7/8/16.
//
//

#ifndef DampingBehavior_h
#define DampingBehavior_h

#include "ForceFieldBehavior.h"
#include "Params.h"
#include "ParticleObject.h"
#include "PhysicsBehavior.h"
#include "PhysicsWorld.h"

class AbstractDampingBehavior
: public AbstractForceFieldBehavior {
public:
  class Params : public ParamsWithEnabled {
  public:
    Params() {
      add(magnitude
          .setKey("magnitude")
          .setName("Magnitude")
          .setValueAndDefault(0.01)
          .setRange(0, 1));
    }

    TParam<float> magnitude;
  };

  AbstractDampingBehavior(const Params& params)
  : _params(params) { }

protected:
  ofVec3f getForceForEntity(PhysicsWorld* world,
                            ParticleObject* entity) override {
    return -entity->velocity() * _params.magnitude();
  }

  const Params& _params;
};

template<typename E>
class DampingBehavior
: public AbstractDampingBehavior {
public:
  DampingBehavior(const Params& params)
  : AbstractDampingBehavior(params) { }

  void applyToWorld(PhysicsWorld* world) override {
    if (!_params.enabled()) {
      return;
    }
    for (auto& entity : world->getEntities<E>()) {
      applyToEntity(world, entity.get());
    }
  }

protected:
  void debugDrawBehavior(PhysicsWorld* world) override {
    if (!_params.enabled()) {
      return;
    }
    for (auto& entity : world->getEntities<E>()) {
      debugDrawEntity(world, entity.get());
    }
  }
};

#endif /* DampingBehavior_h */
