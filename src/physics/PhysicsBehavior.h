//
//  PhysicsBehavior.h
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#ifndef PhysicsBehavior_h
#define PhysicsBehavior_h

#include "Params.h"
#include "State.h"
#include "PhysicsWorld.h"
#include "ParticleObject.h"
#include "Bounds.h"

class AbstractPhysicsBehavior {
public:
  virtual void applyToWorld(PhysicsWorld* world) = 0;
  virtual void debugDraw(PhysicsWorld* world) {
    beginDebugDraw();
    debugDrawBehavior(world);
    endDebugDraw();
  }
protected:
  virtual void beginDebugDraw() { }
  virtual void debugDrawBehavior(PhysicsWorld* world) {}
  virtual void endDebugDraw() { }

  static void drawForceArrow(ofVec3f position,
                             ofVec3f force);
};

class BoundsBehavior
: public AbstractPhysicsBehavior {
public:
  BoundsBehavior(const Params& params, const Bounds& bounds)
  : _params(params)
  , _bounds(bounds) { }

  void applyToWorld(PhysicsWorld* world) override;

private:
  void applyToEntity(PhysicsWorld* world, ParticleObject* entity);

  const Params& _params;
  const Bounds& _bounds;
};


#endif /* PhysicsBehavior_h */
