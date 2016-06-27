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

class ParticleObject : public StandardWorldObject {
public:
  ParticleObject(ofVec3f pos);

  void setInitialCondition(ofVec3f pos, ofVec3f vel);
  void setVelocity(ofVec3f velocity) { _velocity = velocity; }
  void setDamping(float damping) { _damping = damping; }

  void resetForce();
  void addForce(ofVec3f force);
  void addDampingForce();

  virtual void update(const State& state) override;
protected:
  virtual void outputFields(std::ostream& os) const override;
private:
  ofVec3f _velocity;
  ofVec3f _force;
  float _damping;

  friend class AbstractReboundBehavior;
};

class AbstractReboundBehavior {
public:
  class Params : public ValueRange<ofVec3f> {
  public:
    Params(std::string);
  };

  AbstractReboundBehavior(const Params& params);

protected:
  void updateEntity(ParticleObject& entity, const State& state);

private:
  const Params& _params;
};

template<typename T>
class ReboundBehavior
: public AbstractReboundBehavior
, public Behavior<T> {
public:
  ReboundBehavior(const Params& params)
  : AbstractReboundBehavior(params) { }

  void update(T& entity, const State& state) override {
    updateEntity(entity, state);
  }
};

#endif /* ParticleObject_h */
