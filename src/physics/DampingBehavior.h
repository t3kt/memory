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

class DampingBehaviorParams : public ParamsWithEnabled {
public:
  DampingBehaviorParams() {
    add(magnitude
        .setKey("magnitude")
        .setName("Magnitude")
        .setValueAndDefault(2)
        .setRange(0, 20));
  }

  TParam<float> magnitude;
};

class AbstractDampingBehavior
: public AbstractForceFieldBehavior {
public:
  using Params = DampingBehaviorParams;

  AbstractDampingBehavior(Context& context,
                          const Params& params)
  : AbstractForceFieldBehavior(context)
  , _params(params) { }

protected:
  ofVec3f getForceForEntity(ParticleObject* entity) override {
    return -entity->velocity() * _params.magnitude();
  }

  const Params& _params;
};

template<typename E>
class DampingBehavior
: public AbstractDampingBehavior {
public:
  DampingBehavior(Context& context,
                  const Params& params)
  : AbstractDampingBehavior(context, params) { }

  void update() override {
    if (!_params.enabled()) {
      return;
    }
    for (auto& entity : _context.getEntities<E>()) {
      applyToEntity(entity.get());
    }
  }

protected:
  void debugDrawBehavior() override {
    if (!_params.enabled()) {
      return;
    }
    for (auto& entity : _context.getEntities<E>()) {
      if (!_context.highlightedEntities.empty() &&
          !_context.highlightedEntities.containsId(entity->id())) {
        return;
      }
      debugDrawEntity(entity.get());
    }
  }
};

#endif /* DampingBehavior_h */
