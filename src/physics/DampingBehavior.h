//
//  DampingBehavior.h
//  memory
//
//  Created by tekt on 7/8/16.
//
//

#ifndef DampingBehavior_h
#define DampingBehavior_h

#include "../core/Context.h"
#include "../physics/ForceFieldBehavior.h"
#include "../core/Params.h"
#include "../core/ParticleObject.h"
#include "../physics/PhysicsBehavior.h"

class AbstractDampingBehavior
: public AbstractForceFieldBehavior {
public:
  class Params : public ParamsWithEnabled {
  public:
    Params() {
      add(magnitude
          .setKey("magnitude")
          .setName("Magnitude")
          .setValueAndDefault(2)
          .setRange(0, 20));
    }

    TParam<float> magnitude;
  };

  AbstractDampingBehavior(const Params& params)
  : _params(params) { }

protected:
  ofVec3f getForceForEntity(Context& context,
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

  void applyToWorld(Context& context) override {
    if (!_params.enabled()) {
      return;
    }
    for (auto& entity : context.getEntities<E>()) {
      applyToEntity(context, entity.get());
    }
  }

protected:
  void debugDrawBehavior(Context& context) override {
    if (!_params.enabled()) {
      return;
    }
    for (auto& entity : context.getEntities<E>()) {
      if (!context.highlightedEntities.empty() &&
          !context.highlightedEntities.containsId(entity->id())) {
        return;
      }
      debugDrawEntity(context, entity.get());
    }
  }
};

#endif /* DampingBehavior_h */
