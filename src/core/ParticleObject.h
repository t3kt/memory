//
//  ParticleObject.h
//  memory-prototype-2
//
//  Created by tekt on 6/26/16.
//
//

#ifndef ParticleObject_h
#define ParticleObject_h

#include "WorldObject.h"
#include "State.h"
#include "Behavior.h"
#include "Params.h"
#include "Events.h"
#include "Bounds.h"

class ParticleObject : public StandardWorldObject {
public:
  class Params : public ::Params {
  public:
    Params(std::string name);

    ofParameter<float> damping;
  };

  ParticleObject(ofVec3f pos, const Params& params);

  void setInitialCondition(ofVec3f pos, ofVec3f vel);
  void setVelocity(ofVec3f velocity) { _velocity = velocity; }

  void resetForce();
  void addForce(ofVec3f force);
  void addDampingForce();

  virtual void update(const State& state) override;
protected:
  virtual void outputFields(std::ostream& os) const override;
private:
  const Params& _params;
  ofVec3f _velocity;
  ofVec3f _force;

  friend class AbstractReboundBehavior;
};

class AbstractReboundBehavior {
public:
  AbstractReboundBehavior(const Bounds& bounds);

protected:
  bool updateEntity(ParticleObject& entity, const State& state);

private:
  const Bounds& _bounds;
};

template<typename E>
class ReboundBehavior
: public AbstractReboundBehavior
, public Behavior<E> {
public:
  ReboundBehavior(const Bounds& bounds)
  : AbstractReboundBehavior(bounds) { }

  ofxLiquidEvent<EntityEventArgs<E>> entityRebounded;

  void update(E& entity, const State& state) override {
    if (updateEntity(entity, state)) {
      EntityEventArgs<E> e(state, entity);
      entityRebounded.notifyListeners(e);
    }
  }
};

#endif /* ParticleObject_h */
