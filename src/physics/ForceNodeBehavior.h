//
//  ForceNodeBehavior.h
//  memory
//
//  Created by tekt on 9/18/16.
//
//

#ifndef ForceNodeBehavior_h
#define ForceNodeBehavior_h

#include "../core/Context.h"
#include "../core/NodeEntity.h"
#include "../core/ObjectManager.h"
#include "../core/Params.h"
#include "../core/ParticleObject.h"
#include "../core/ValueSequence.h"
#include "../physics/PhysicsBehavior.h"

class VortexForceBehaviorParams : public ParamsWithEnabled {
public:
  VortexForceBehaviorParams() {
    add(radius
        .setKey("radius")
        .setName("Radius")
        .setParamRanges(0, 200)
        .setParamValuesAndDefaults(10, 80));
    add(force
        .setKey("force")
        .setName("Force"));
    force.setValueRanges(-15, 15);
    force.setLengthRanges(0, 1);
    force.startValue.setValueAndDefault(10);
    force.values[0].setValueAndDefault(6);
    force.values[1].setValueAndDefault(2);
    force.lengths[0].setValueAndDefault(0.5);
    force.lengths[1].setValueAndDefault(1);
    add(magnitude
        .setKey("magnitude")
        .setName("Magnitude")
        .setRange(0, 100)
        .setValueAndDefault(10));
  }
  RandomValueSupplier<float> radius;
  ValueSequence<float, 2> force;
  TParam<float> magnitude;
};

class VortexForceNode
: public NodeEntity {
public:
  VortexForceNode(ofVec3f pos,
                  ofVec3f axis,
                  float radius)
  : NodeEntity(pos)
  , _axis(axis)
  , _radius(radius) { }

  std::string typeName() const { return "VortexForceNode"; }

  const ofVec3f& axis() const { return _axis; }
  float radius() const { return _radius; }

private:
  ofVec3f _axis;
  float _radius;
};

class AbstractVortexForceNodeBehavior
: public AbstractPhysicsBehavior {
public:
  using Params = VortexForceBehaviorParams;

  AbstractVortexForceNodeBehavior(Context& context,
                                  const Params& params)
  : AbstractPhysicsBehavior(context)
  , _params(params) { }

  std::shared_ptr<VortexForceNode> spawnNode(ofVec3f pos);

protected:
  void cullDeadNodes();
  void applyToEntity(ParticleObject* entity);
  ofVec3f calculateForce(const VortexForceNode& node,
                         ParticleObject* entity);
  void debugDrawEntity(ParticleObject* entity);
  void debugDrawNodes();

  const Params& _params;
  ObjectManager<VortexForceNode> _nodes;
};

template<typename E>
class VortexForceNodeBehavior
: public AbstractVortexForceNodeBehavior {
public:
  VortexForceNodeBehavior(Context& context,
                          const Params& params)
  : AbstractVortexForceNodeBehavior(context, params) { }

  void update() override {
    if (!_params.enabled.get()) {
      return;
    }
    cullDeadNodes();
    for (auto& entity : _context.getEntities<E>()) {
      applyToEntity(entity.get());
    }
  }
  void debugDrawBehavior() override {
    if (!_params.enabled.get()) {
      return;
    }
    for (auto& entity : _context.getEntities<E>()) {
      debugDrawEntity(entity.get());
    }
    debugDrawNodes();
  }
};

#endif /* ForceNodeBehavior_h */
