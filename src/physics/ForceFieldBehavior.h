//
//  ForceFieldBehavior.h
//  memory
//
//  Created by tekt on 7/7/16.
//
//

#ifndef ForceFieldBehavior_h
#define ForceFieldBehavior_h

#include "Params.h"
#include "ParticleObject.h"
#include "PhysicsBehavior.h"
#include "PhysicsWorld.h"

class AbstractForceFieldBehavior
: public AbstractPhysicsBehavior {
public:

protected:
  virtual ofVec3f getForceForEntity(PhysicsWorld* world,
                                    ParticleObject* entity) = 0;
  void applyToEntity(PhysicsWorld* world, ParticleObject* entity);
  void debugDrawEntity(PhysicsWorld* world, ParticleObject* entity);
  void beginDebugDraw() override;
  void endDebugDraw() override;
};

class AbstractNoiseForceFieldBehavior
: public AbstractForceFieldBehavior {
public:
  class Params : public ParamsWithEnabled {
  public:
    Params() {
      add(scale
          .setKey("scale")
          .setName("Scale")
          .setValueAndDefault(1)
          .setRange(0, 4));
      add(rate
          .setKey("rate")
          .setName("Rate")
          .setValueAndDefault(0.1)
          .setRange(0, 0.5));
      add(magnitude
          .setKey("magnitude")
          .setName("Magnitude")
          .setValueAndDefault(0.001)
          .setRange(0, 0.05));
    }

    TParam<float> scale;
    TParam<float> rate;
    TParam<float> magnitude;
  };

  AbstractNoiseForceFieldBehavior(const Params& params)
  : _params(params) { }

protected:
  ofVec3f getForceForEntity(PhysicsWorld* world,
                            ParticleObject* entity) override;

  const Params& _params;
};

template<typename E>
class NoiseForceFieldBehavior
: public AbstractNoiseForceFieldBehavior {
public:
  NoiseForceFieldBehavior(const Params& params)
  : AbstractNoiseForceFieldBehavior(params) { }

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

#endif /* ForceFieldBehavior_h */
